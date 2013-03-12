BindingsGen = {}
BindingsGen.name = "BindingsGen"

project "BindingsGen"

  kind "ConsoleApp"
  language "C#"
  location "."

  files { "**.cs", "./*.lua" }

  dependson { "Bridge", "Generator", "Parser" }

  links
  {
    "System",
    path.join(depsdir, "cxxi", "build", action, "lib", "Bridge"),
    path.join(depsdir, "cxxi", "build", action, "lib", "Generator"),
  }


