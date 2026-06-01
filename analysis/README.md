# Analysis Workspace

Python analysis consumes structured outputs from C++ projects and produces
plots, tables, and reports.

Preferred exchange boundary:

```text
projects/<name>/tests or benchmarks
  -> artifacts/<name>/*.json or *.csv
  -> analysis scripts
  -> artifacts/<name>/plots and reports
```

Ordinary C++ unit-test builds must not require the Python analysis image.

