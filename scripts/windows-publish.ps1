[CmdletBinding()]
param (
    [string] $archiveName, [string] $targetName
)
# 外部环境变量包括:
# VCINSTALLDIR: 'C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC'
# archiveName: ${{ matrix.qt_ver }}-${{ matrix.qt_arch }}
# winSdkDir: ${{ steps.build.outputs.winSdkDir }}
# winSdkVer: ${{ steps.build.outputs.winSdkVer }}
# vcToolsInstallDir: ${{ steps.build.outputs.vcToolsInstallDir }}
# vcToolsRedistDir: ${{ steps.build.outputs.vcToolsRedistDir }}
# msvcArch: ${{ matrix.msvc_arch }}

$scriptDir = $PSScriptRoot
$currentDir = Get-Location
Write-Host "currentDir" $currentDir
Write-Host "scriptDir" $scriptDir

function Main() {

    New-Item -ItemType Directory $archiveName
    # 拷贝exe
    Copy-Item release\$targetName $archiveName\
    # 拷贝依赖
    windeployqt --qmldir . --plugindir $archiveName\plugins --no-translations --compiler-runtime $archiveName\$targetName
    # 删除不必要的文件
    $excludeList = @("*.qmlc", "*.ilk", "*.exp", "*.lib", "*.pdb")
    Remove-Item -Path $archiveName -Include $excludeList -Recurse -Force
    # 拷贝vcRedist dll
    $redistDll="{0}{1}\*.CRT\*.dll" -f $env:vcToolsRedistDir.Trim(),$env:msvcArch
    Copy-Item $redistDll $archiveName\
    # 拷贝WinSDK dll
    $sdkDll="{0}Redist\{1}ucrt\DLLs\{2}\*.dll" -f $env:winSdkDir.Trim(),$env:winSdkVer.Trim(),$env:msvcArch
    Copy-Item $sdkDll $archiveName\
    # 打包zip
    Compress-Archive -Path $archiveName $archiveName'.zip'
}

if ($null -eq $archiveName || $null -eq $targetName) {
    Write-Host "args missing, archiveName is" $archiveName ", targetName is" $targetName
    return
}
Main


