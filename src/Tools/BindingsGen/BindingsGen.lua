BindingsGen = {}
BindingsGen.name = "BindingsGen"

project "BindingsGen"

  kind "ConsoleApp"
  language "C#"
  location "."

  files { "**.cs", "./*.lua" }

  dependson { "CppSharp.AST", "CppSharp.Generator", "CppSharp.Parser" }

  links
  {
    "System",
    path.join(depsdir, "CppSharp", "build", action, "lib", "CppSharp.AST"),
    path.join(depsdir, "CppSharp", "build", action, "lib", "CppSharp.Generator"),
  }


