@echo off
chcp 65001 >nul
cls

@echo off
pushd "%~dp0"
cd ..
bin\lua.exe lua\getFilesOfDir.lua "C:\Users\A\Desktop\work\leaper-main\assets" "temp\leaper_all.txt"

@REM 生成 meta 文件对应关系 uuid

py -3 Python\CocosMetaMap.py --txt temp\leaper_all.txt



setlocal EnableDelayedExpansion
set client_dir=C:\Users\A\Desktop\work\leaper-main\assets
set LUA_PARAM=return { ^
exclude = { '.git', '.svn', '.vscode' } , ^
ext = { '.prefab' }, ^
}
bin\lua.exe lua\getFilesOfDirWithConfig.lua "%client_dir%" "temp\leaper_prefab.txt" "!LUA_PARAM!"

py -3 Python\CocosRefByPrefab.py --txt temp\leaper_prefab.txt --output temp/prefab_uuid_set.json

py -3 Python\CocosGetFileByUuid.py --uuid temp/prefab_uuid_set.json --uuid_file_map temp/uuid_file_map.json

py -3 Python\CocosDeleteFiles.py --txt temp\leaper_all.txt

py -3 Python\CocosPrefabTree.py --txt temp\code_prefab.txt --output temp/all_prefab_uuid_set.json --uuid_file_map temp/uuid_file_map.json

py -3 Python\CocosDeletePrefab.py --txt temp\leaper_all.txt

py -3 Python\CocosDelete.py --config temp/delete_prefab_files.json
py -3 Python\CocosDelete.py --config temp/delete_files.json


popd

pause





