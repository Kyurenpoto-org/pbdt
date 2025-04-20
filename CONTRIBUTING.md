# Contribution Guidelines

Thank you for your interest in contributing to this project! If you would like to contribute, please follow the guidelines below.

## Contribution Process

1. Fork this repository.
2. Create a new branch.
3. Commit your changes.
4. Push the branch.
5. Create a Pull Request.

## Writing Tests

This project uses property-based testing to ensure code quality. When writing tests, please check below.

### Checklist

- **Use Generators**: Use generators located in `tests/includes/generators` to create the necessary test data.
- **Define Properties**: Properties located in `tests/includes/properties` should express a verification flow that checks whether the requirements are satisfied.
- **Comply with Requirements Interface**: Requirements must have a `constexpr` interface to ensure that properties are satisfied at both compile time and runtime.
- **Group Test Suites**: Group tests for the same property into test suites.

## Automation with AI

You can automate some checklists using prompts to ensure that all requirements are met efficiently. See [docs/prompt.md](docs/prompt.md) for more details.

## Contributor Code of Conduct

All contributors should respect one another, provide constructive feedback, and strive to create an inclusive environment.

## Questions

If you have any questions about contributing, please open an issue or contact us directly.
