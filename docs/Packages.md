Package Metadata
----------------

`name` (*required*)

Name of the package.

---
`version` (*required*)

Version of the package.

The version should be parseable as [Semantic Versioning](http://semver.org/).

---
`dependencies`

Describes the dependencies of the package.

```json
{ "dependencies" :
  { "foo" : "1.0.0 - 2.9999.9999"
  , "bar" : ">=1.0.2 <2.1.2"
  , "baz" : ">1.0.2 <=2.3.4"
  , "boo" : "2.0.1"
  , "qux" : "<1.0.0 || >=2.3.1 <2.4.5 || >=2.5.2 <3.0.0"
  , "asd" : "http://asdf.com/asdf.tar.gz"
  , "til" : "~1.2"
  , "elf" : "~1.2.3"
  , "two" : "2.x"
  , "thr" : "3.3.x"
  }
```

---
`license`

Describes the license used for your package.

---
`projects`

Describes the projects provided by the package.

```json
{ "projects" :
  { "foo" : "1.0.0 - 2.9999.9999"
  , "bar" : ">=1.0.2 <2.1.2"
  }
```

---
`toolset`

Describes the valid toolsets supported by this package.

Valid values: `msvc`, `clang`, `gcc`, `mono`, `net`

A toolset can also be versioned by appending a number to its name.

---
`platform`

Describes the valid platforms supported by this package.

Valid values: `win`, `osx`, `linux`

A platform can also be versioned by appending a number to its name.

---
`configuration`

Describes the build configurations supported by this package.

Valid values: `debug`, `release`

Package Manager
---------------

The package manager is responsible for downloading dependencies for the
project.

Package Server
--------------

The package server provides an API 

`foo-1.2-msvc17-windows.zip`

HTTP
Flood Services
Thrift


