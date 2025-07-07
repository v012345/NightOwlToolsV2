本地不使用工具生成一个 git 仓库
git init --bare D:\git-server\myproject.git
将这个文件夹共享出来（右键 → 属性 → 共享 → 高级共享）
假设共享名是 git-server，机器名或 IP 是 192.168.1.20，你可以用：
git clone //192.168.1.20/git-server/myproject.git