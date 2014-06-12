BindingsGen = {}
BindingsGen.name = "BindingsGen"

project "BindingsGen"

  kind "ConsoleApp"
  language "C#"
  uuid "40093510-B258-40A1-B935-5F0783A647DC"

  location "."

  files { "**.cs", "*.lua" }

  dependson { "CppSharp.AST", "CppSharp.Generator", "CppSharp.Parser" }

  links
  {
    "System",
    path.join(depsdir, "CppSharp", "build", action, "lib", "CppSharp.AST"),
    path.join(depsdir, "CppSharp", "build", action, "lib", "CppSharp.Generator"),
  }


