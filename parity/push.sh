MESSAGE="${1:-Code pushed via CLI}"

git add .
git commit -m "$MESSAGE"
git push
