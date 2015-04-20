REF=$(git merge-base origin/master HEAD)
git rebase -i $REF
sleep 5