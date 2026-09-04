<#
.SYNOPSIS
Builds and runs the analysis Docker image for the repository.

.DESCRIPTION
Builds the Docker image defined by docker/Dockerfile.analysis, then starts a
container and verifies the bundled Python runtime is available.

.PARAMETER ImageName
Docker image tag to build and run.

.EXAMPLE
Get-Help .\tools\run-analysis-docker.ps1 -Detailed

.EXAMPLE
.\tools\run-analysis-docker.ps1 -ImageName lra-numerical-analysis
#>
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
