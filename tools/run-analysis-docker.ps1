param(
    [string]$ImageName = 'lra-numerical-analysis'
)

$ErrorActionPreference = 'Stop'

$repo = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path

docker build -f (Join-Path $repo 'docker/Dockerfile.analysis') -t $ImageName $repo
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

docker run --rm -v "${repo}:/workspace" -w /workspace $ImageName python --version

if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}
