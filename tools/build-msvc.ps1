param(
    [ValidateSet('Debug', 'Release')]
    [string]$Configuration = 'Debug'
)

$ErrorActionPreference = 'Stop'

$preset = if ($Configuration -eq 'Release') { 'msvc-release' } else { 'msvc-debug' }
$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"

if (-not (Test-Path $vswhere)) {
    throw "vswhere.exe was not found. Install Visual Studio 2022 with C++ tools."
}

$vcvars = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -find 'VC\Auxiliary\Build\vcvars64.bat' | Select-Object -First 1

if (-not $vcvars) {
    throw "vcvars64.bat was not found. Install the Visual Studio C++ workload."
}

$repo = Resolve-Path (Join-Path $PSScriptRoot '..')
$exe = if ($Configuration -eq 'Release') {
    "build\msvc-release\projects\hello-one\Release\hello_one.exe"
} else {
    "build\msvc-debug\projects\hello-one\Debug\hello_one.exe"
}

$command = "call `"$vcvars`" && cd /d `"$repo`" && cmake --preset $preset && cmake --build --preset $preset && `"$exe`" && ctest --preset $preset"
cmd.exe /c $command

if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}
