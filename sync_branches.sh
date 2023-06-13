#!/bin/bash

DIR=builder

WORKFLOW=../build_workflow.yml

cd $DIR

echo ''
all_branches=$(git branch -r | sed -e '/origin\/HEAD -> /d')

local_branches=$(echo "$all_branches" | sed -e '/origin/!d' -e 's/origin\///g')
upstream_branches=$(echo "$all_branches" | sed -e '/upstream/!d' -e 's/upstream\///g')

update_branches=$(echo -e "$local_branches\n$upstream_branches" | sort | uniq -d)
new_branches=$(comm -2 -3 <(echo "$upstream_branches" | sort) <(echo "$update_branches" | sort))
old_branches=$(comm -2 -3 <(echo "$local_branches" | sort) <(echo "$update_branches" | sort))

echo "New Branches:"
echo "$new_branches"
echo "Branches to check for updates:"
echo "$update_branches"
echo 'Old Branches:'
echo "$old_branches"

echo -e "Starting...\n\n"

# All new branches need to be synced
sync_list=$new_branches

checksync() {
    local commits_behind=$(git rev-list --count origin/$1..upstream/$1)
    local commits_ahead=$(git rev-list --count upstream/$1..origin/$1)

    if [ "$commits_ahead" == "1" ] && [ "$commits_behind" == "0" ]
    then
        echo "$br is up to date!"
    else
        echo "$br needs syncing (Behind: $commits_behind, Ahead: $commits_ahead)"
        sync_list=$(echo "$br ${sync_list[@]}")
    fi
}

# Check to see which existing branches need to be synced
for br in $update_branches
do
    checksync $br
done

for old in $old_branches
do
    echo "TODO: Old - $old"
done

dosync() {
    echo "Syncing upstream/$1 to origin/$1"
    git checkout -B $1 upstream/$1
    mkdir -p .github/workflows
    cp $WORKFLOW .github/workflows/build.yml
    git add .github/workflows/build.yml
    git commit -m "Setup Github Actions"
    git push --force -u origin $1
}

for sync in $sync_list
do
    dosync $sync
done

test_list=$(echo "$sync_list")
echo $test_list
for branch in $test_list
do
    checksync $branch
done