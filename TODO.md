PluginArchitect — TODO / Progress Log
✅ DONE (v0.1 – Foundation & Validation)
Core Architecture

Designed clean module separation
Editor module (PluginArchitect) for UX
Runtime module (PluginArchitectRuntime) for core logic
Enforced one-direction dependency
Editor → Runtime
Runtime has no Editor dependencies

Logging
    Implemented Editor-side logging system
    Verified logging works in StartupModule and console commands
    Removed cross-module logging misuse (Runtime no longer depends on Editor logs)

Descriptor System (Runtime)
    Implemented FPluginDescriptor
    Implemented FPluginModuleDescriptor
    Ensured single source of truth for descriptors (Runtime only)
    Fixed linker issues caused by duplicate / mismatched descriptor types

Console UX (Editor)
    Registered Editor console command:
    PluginArchitect.CreatePlugin <PluginName>
    Verified command works in Unreal Editor console
    Console command correctly parses arguments
    Console command delegates work correctly (FConsoleCommandWithArgsDelegate)

Generator Core (Runtime)
    Created FPluginSkeletonGenerator
    Generator validates:

Plugin name

Target plugin directory

 Generator logs internal state (name, root path, module count)

 Generator performs NO filesystem side effects (validation only)

 Generator successfully callable from Editor module

Stability / Tooling

 Resolved Unreal module initialization issues

 Resolved Windows Smart App Control / DLL blocking issues

 Clean rebuild process validated

 Unreal Editor opens cleanly with plugin enabled





🚧 NEXT (v0.2 – Filesystem Generation)
Phase 2: Directory Creation (Next Immediate Task)

 Create FPluginLayoutBuilder (Runtime)

 Hard-fail if plugin folder already exists

 Create directory structure:

Plugins/<PluginName>/
├─ Source/
│  └─ <PluginName>Runtime/
│     ├─ Public/
│     └─ Private/
└─ Resources/


 Log every directory creation step

 Integrate layout builder into FPluginSkeletonGenerator

Phase 3: File Generation (After Directories)

 Generate .uplugin file from FPluginDescriptor

 Generate <PluginName>Runtime.Build.cs

 Generate Runtime module .h and .cpp

 Ensure generated plugin loads after editor restart

🔮 FUTURE (Not Now, But Planned)
UX Improvements

 Ask user what to do if plugin folder already exists

overwrite

rename

cancel

 Optional Editor UI (no Slate until generator is stable)

Advanced Features

 Editor module generation

 Multiple modules per plugin

 Dependency selection

 Preset templates

 Validation UI

 Marketplace-ready packaging