<!--
GENERATED FILE — DO NOT EDIT BY HAND.

Source repo: wsollers/lra-governance
Source commit: d98bb51fc80e683b38a9d1e76f4a0c91037ede0a
Generated from:
- docs/governance/...
- docs/architecture/...
- docs/governance/repo-overlays/lra-numerical-analysis.md

Regenerate from lra-governance.
Emergency downstream edits must be ported upstream before the next sync.
-->

# LRA Repository Instructions

This file is intended for `.github/instructions/lra.instructions.md`. Keep it
concise and refer to canonical governance docs rather than copying large docs.

## Global Agent Rules

- Treat generated instruction files as derived artifacts.
- Follow the owning repository boundary for every task.
- Do not include secrets, credentials, tokens, or machine-local private values.
- Do not modify mathematical content during governance or wrapper-generation tasks.
- Do not touch `Learning-Real-Analysis/scripts/`.
- Port emergency downstream instruction repairs back to `lra-governance`.

## Repo Overlay

# lra-numerical-analysis Overlay

Stub overlay for numerical analysis work.

Owned concerns:

- numerical methods,
- computational C++ experiments,
- benchmarks,
- plotting,
- LaTeX-ready numerical reports.

## Agent Scope

Numerical-analysis guidance applies only to `lra-numerical-analysis`. It may
cover numerical C++ experiments, benchmarking, plotting, and report generation,
but it must not be injected into volume content instructions.

This repo does not own Lean formalization, NURBS/Vulkan simulation, or shared
LaTeX infrastructure.

## Provider Notes

Keep provider-specific guidance concise and defer durable policy to governance docs.
