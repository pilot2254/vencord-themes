# DBD Internal

Internal x64 DLL using CRYMORE's shared DX11 overlay and ESP renderer. Insert opens the menu; the ESP renderer accepts validated camera and actor snapshots through `esp::publish`.

## Build

From the CRYMORE workspace root:

```powershell
& 'C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe' dbd-internal\cheat\cheat.vcxproj -p:Configuration=Release -p:Platform=x64
```

The project compiles the CRYMORE C++20 sources directly from `..\crymore-lib` and uses its bundled ImGui. No separate package restore is required.

## Current status

- Overlay DLL entry point, menu, survivor/killer ESP configuration, and objective marker configuration are wired.
- The ESP draw path consumes snapshots, but no game-data provider calls `esp::publish` yet. It displays a waiting status and does not read game memory or draw fabricated actors.
- The local Steam manifest reported installed BuildID `23993349` and TargetBuildID `24249604`. These are manifest values; no symbols, game structures, or addresses have been validated against the executable. Update DBD before taking a build-specific dump.
- Runtime behavior has not been tested in DBD.

## Next required input

Provide a fresh SDK/dump from the exact installed build (or identify the exact game build you want targeted). The provider must validate the world, camera, actor classes, and coordinates before publishing a ready snapshot; until then the provider stays disabled.
