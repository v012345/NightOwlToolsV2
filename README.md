## 下载项目
```console
git clone https://github.com/v012345/NightOwlToolsV2.git
cd NightOwlToolsV2
git submodule init
git submodule update
```

todo 要测试!!!
## 删子模块
```git bash
$ git submodule deinit -f tool_scripts/extensions/
$ git rm -r tool_scripts/extensions/
$ rm -rf .git/modules/tool_scripts/extensions/
```

## 加入子模块
```
git submodule add git@github.com:Egor-Skriptunoff/pure_lua_SHA.git tool_scripts/extensions/pure_lua_SHA

```

## Refresh .gitignore
```
git rm -r --cached .
git add .
git commit -m "Refresh .gitignore"
```