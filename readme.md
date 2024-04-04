# main


- 创建新的孤儿分支 `git checkout --orphan branchname`
- 提交到远程分支(初次) `git push --set-upstream origin branchname`
- 查看分支
  - 查看本地分支 `git branch`
  - 查看远程分支 `git branch -r`
  - 查看所有分支 `git branch -a`
- 临时存储
  - 存储 `git stash save`
  - 恢复 `git stash pop`
- 暂存
  - 删除暂存区 `git rm -r --cached . -f`
  - 添加到暂存 `git add .`
  - 查看状态 `git status`
