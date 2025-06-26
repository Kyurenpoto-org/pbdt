# `pbdt::bdd` Constraints Guide

This document explains the roles of the concepts under the `pbdt::bdd` namespace,
as well as the inclusion and mutual exclusion relationships between these concepts.

## Roles of Each Concept

The following concepts are defined in `../include/pbdt/constraints.hpp`:

- **CallableTarget**  
  Represents a callable target that can be used in the "GIVEN" phase of BDD.
- **CallableProperty**  
  Represents a property expected in the "THEN" phase.  
  Must accept two arguments and return a `TestContext` type.
- **RangeDomain**  
  Represents a set of input values (domain) that can be used in the "WHEN" phase.

## Inclusion and Mutual Exclusion Relationships

```
exstd::Callable
  └─ pbdt::bdd::CallableTarget
        └─ pbdt::bdd::CallableProperty

pbdt::bdd::CallableTarget ⊥ pbdt::bdd::RangeDomain
```


- **exstd::Callable ⊃ pbdt::bdd::CallableTarget**  
  `exstd::Callable` includes all callable types, and `CallableTarget` is a subset of this.  
  → This inclusion relationship is verified via property-based testing
  using the `InclusiveTypeValidation` class in [`inclusive.hpp`](../tests/include/properties/constraints/inclusive.hpp).

- **pbdt::bdd::CallableTarget ⊃ pbdt::bdd::CallableProperty**  
  `CallableProperty` includes only those types that are callable, take two arguments, and return a `TestContext` type.  
  → This inclusion relationship is also verified
  using the `InclusiveTypeValidation` class in [`inclusive.hpp`](../tests/include/properties/constraints/inclusive.hpp).

- **pbdt::bdd::CallableTarget ⊥ pbdt::bdd::RangeDomain**  
  Callable types and range types are essentially mutually exclusive.  
  → This mutual exclusion relationship is verified via property-based testing
  using the `MutualExclusiveTypeValidation` class in
  [`mutual-exclusive.hpp`](../tests/include/properties/constraints/mutual-exclusive.hpp).
