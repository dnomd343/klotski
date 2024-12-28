# Klotski Changelog

All notable changes to this project will be documented in this file.

The format is based on [*Keep a Changelog*](https://keepachangelog.com/en/1.0.0/), and this project adheres to [*Semantic Versioning*](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.0.2] - 2024-12-28

### Added

- New `analyse` module for analyzing BFS graph structures.

- More test suites and performance benchmark suites.

- More Python and Rust interfaces.

### Changed

- Performance optimization of some functions.

- Internal structure and interfaces adjustment of core.

- Using `Layout` instead of `CommonCode` in external interfaces.

### Fixed

- Avoid compiling C++ code when building on `docs.rs`.

## [0.0.1] - 2024-11-16

### Added

- Basic klotski functions, including `codec`, `cases`, `group`, `mover` and `fast_cal` modules.

- Basic test suites and performance benchmark suites.

- Preliminary Python interfaces for preview.

- Preliminary Rust interfaces for preview.

[unreleased]: https://github.com/dnomd343/klotski/compare/v0.0.2...HEAD
[0.0.2]: https://github.com/dnomd343/klotski/releases/tag/v0.0.2
[0.0.1]: https://github.com/dnomd343/klotski/releases/tag/v0.0.1
