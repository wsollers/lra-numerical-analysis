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
