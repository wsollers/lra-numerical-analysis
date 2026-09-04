<#
.SYNOPSIS
Builds and runs the Clang Docker development image for the repository.

.DESCRIPTION
Builds the Docker image defined by docker/Dockerfile.clang, then runs the
repository build, sample executable, and test suite inside the container.

.PARAMETER ImageName
Docker image tag to build and run.

.EXAMPLE
Get-Help .\tools\build-docker-clang.ps1 -Detailed

.EXAMPLE
.\tools\build-docker-clang.ps1 -ImageName lra-numerical-clang
#>
param(
    [string]$ImageName = 'lra-numerical-clang'
)

$ErrorActionPreference = 'Stop'

$repo = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path

docker build -f (Join-Path $repo 'docker/Dockerfile.clang') -t $ImageName $repo
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

docker run --rm -v "${repo}:/workspace" -w /workspace $ImageName `
    bash -lc "cmake -S . -B build/docker-clang -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++-18 -DLRA_NUM_ENABLE_SANITIZERS=OFF && cmake --build build/docker-clang && ./build/docker-clang/projects/hello-one/hello_one && ctest --test-dir build/docker-clang --output-on-failure"

if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}
