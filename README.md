# 3ds Max Modal Transforms for Unreal Engine 5

An editor-only Unreal Engine plugin that adds keyboard-first modal transforms using the familiar **W / E / R** layout:

- Press a transform key once to begin.
- Move the mouse to update the transform without holding a mouse button.
- Add an axis or plane constraint at any time.
- Confirm the result or cancel and restore the exact starting transform.

The existing Unreal transform gizmo remains available. Modal input starts only while the cursor is over an editor viewport and is ignored while typing, while a menu is open, or during PIE.

> This project is an independent community plugin. It is not affiliated with or endorsed by Autodesk or Epic Games.

## 1. Installation

### Method 1: Git clone (recommended)

1. Navigate to the Unreal project root containing your `.uproject` file.
2. Create a `Plugins` folder if it does not already exist.
3. Open a terminal inside `Plugins`.
4. Clone this repository:

   ```powershell
   git clone https://github.com/Figix/3dMaxModalTransformsUE5.git 3DMaxEditorControlsUE5
   ```

5. Open the Unreal project and allow Unreal Engine to rebuild the plugin modules.
6. If the plugin is not active, open **Edit → Plugins**, search for **3ds Max Editor Controls**, enable it, and restart the editor.

### Method 2: ZIP download

1. Download the repository ZIP from GitHub and extract it.
2. Rename the extracted directory to `3DMaxEditorControlsUE5` if necessary.
3. Move it into `<YourProject>/Plugins/`.
4. Open the Unreal project and allow Unreal Engine to rebuild the plugin modules.
5. If necessary, enable **3ds Max Editor Controls** under **Edit → Plugins** and restart the editor.

This repository contains source code rather than checked-in build products. A Windows Unreal C++ toolchain is therefore required. C++ developers can regenerate project files from the `.uproject` context menu and build the Editor target from Visual Studio or Rider.

## 2. Features

- **W / E / R modal transforms** — start Translate, Rotate, or Scale directly from the keyboard.
- **Button-free mouse control** — continue transforming from mouse movement without holding a mouse button.
- **Mid-session tool switching** — switch between Translate, Rotate, and Scale without ending the session.
- **Infinite cursor wrapping** — the cursor wraps at viewport edges so long movements are not limited by screen size.
- **Axis and plane constraints** — use `X`, `Y`, `Z`, or their Shift-modified plane variants.
- **World and local axes** — press an axis key again to toggle the constraint between world and local space.
- **Numeric input** — enter exact distances, angles, or scale values with unit-aware expression parsing.
- **Trackball rotation** — press `E` again during Rotate to toggle freeform trackball rotation.
- **Duplicate and move** — press `Shift + D` to duplicate the actor selection and immediately begin Translate.
- **Precision and snapping** — hold `Shift` for fine adjustment and `Ctrl` to invert the current viewport snapping state.
- **Multi-context architecture** — includes adapters for Level Editor actors, Blueprint SCS components, and Control Rig elements.
- **Undo/Redo** — each modal session uses Unreal's transaction system; cancellation restores the initial state.
- **Input safety** — transform hotkeys are intercepted only over an editor viewport, not in text fields, menus, PIE, or while the right mouse button is already held for camera navigation.

The current Phase 1 acceptance target is the single-Actor Level Editor workflow. Multi-selection, Blueprint component, and Control Rig support are present in the inherited implementation but should be treated as expanding test coverage rather than the completed Phase 1 guarantee.

> If constraint lines do not appear immediately, wait for background shader compilation to finish.

## 3. Usage and hotkeys

| Action | Shortcut | Description |
| --- | --- | --- |
| Translate | `W` | Move the selection relative to the current view. |
| Rotate | `E` | Rotate the selection relative to the view angle. |
| Trackball Rotate | `E`, then `E` | Toggle freeform trackball rotation. |
| Scale | `R` | Scale uniformly by default. |
| Switch Tool | `W` / `E` / `R` | Change the active transform during a modal session. |
| Duplicate and Move | `Shift + D` | Duplicate selected actors and begin Translate. |
| Lock World Axis | `X`, `Y`, or `Z` | Constrain the transform to one world axis. |
| Lock Local Axis | Press the same axis again | Toggle the active constraint to local space. |
| Lock Plane | `Shift + X/Y/Z` | Exclude that axis; for example, `Shift + Z` constrains to the XY plane. |
| Numeric Input | `0–9`, `.`, `-`, units | Enter an exact transform value. |
| Confirm | Left Click, `Enter`, or `Space` | Commit the transform transaction. |
| Cancel | Right Click or `Esc` | Restore the initial transform and cancel the transaction. |
| Precision Mode | Hold `Shift` | Reduce mouse influence for fine adjustments. |
| Toggle Snapping | Hold `Ctrl` | Temporarily invert viewport snapping. |

### Viewport camera priority

Hold the **Right Mouse Button first**, then press `W`, `E`, or another viewport navigation key. The plugin passes those keys through so Unreal's normal fly-camera controls take priority. Release RMB and press `W`, `E`, or `R` normally to start a modal transform.

If a modal transform has already started, Right Click keeps its existing meaning: cancel the transform and restore the initial state.

Default shortcuts are rebindable under **Edit → Editor Preferences → General → Keyboard Shortcuts**. Search for **3ds Max Editor Controls**.

## 4. Settings and customization

Open:

**Edit → Editor Preferences → Plugins → 3ds Max Editor Controls**

Available settings include:

- X/Y/Z constraint colors
- axis-line thickness and dash scale
- HUD scale and placement
- custom cursor scale
- precision-mode multiplier
- trackball sensitivity

## 5. Coexistence with BlenderEditorControlsUE5

This plugin does not modify the installed `BlenderEditorControlsUE5` plugin. Both can remain in the same project's `Plugins` directory.

When both are enabled, this plugin registers at a slightly higher input-preprocessor priority. The shared `R` shortcut therefore starts this plugin's **Scale** operation rather than the Blender plugin's **Rotate** operation. Disable either plugin when you need its full shortcut set without ambiguity.

## 6. Architecture overview

```text
Source/
└── ThreeDMaxEditorControlsPlugin/
    ├── ThreeDMaxEditorControlsPlugin.Build.cs
    ├── Private/
    │   ├── ControlRig/
    │   │   └── ControlRigSelectionHelper.cpp
    │   ├── Input/
    │   │   ├── InputProcessor.cpp
    │   │   └── Numeric/
    │   │       ├── NumericInputProcessor.cpp
    │   │       └── Helpers/
    │   │           ├── NumericParser.cpp
    │   │           └── UnitFormatter.cpp
    │   ├── Pivots/
    │   │   ├── ActorPivot.cpp
    │   │   ├── ControlRigPivot.cpp
    │   │   └── SCSPivot.cpp
    │   ├── Tools/
    │   │   ├── MoveTool.cpp
    │   │   ├── RotateTool.cpp
    │   │   ├── ScaleTool.cpp
    │   │   └── ToolBase.cpp
    │   ├── UI/
    │   │   ├── ThreeDMaxAxisLockGizmoComponent.cpp
    │   │   └── TransformHUD.cpp
    │   ├── Utils/
    │   │   └── MathHelpers.cpp
    │   ├── ThreeDMaxControlsCommands.cpp
    │   ├── ThreeDMaxEditorControls.cpp
    │   └── TransformSession.cpp
    └── Public/ (headers mirroring the Private structure)
```

- **InputProcessor** registers a Slate input preprocessor, filters editor context, and starts transform sessions before the viewport handles W/E/R.
- **TransformSession** is the modal state machine. It owns the active tool, initial mouse/camera state, target pivot, numeric processor, and `FScopedTransaction` lifecycle.
- **Tools** calculate Translate, Rotate, and Scale results from accumulated mouse movement and numeric input.
- **Pivots** abstract target-specific transform access for actors, Blueprint SCS components, and Control Rig elements.
- **Input/Numeric** parses values, units, signs, reciprocals, and multi-slot input.
- **UI** displays live values and renders axis/plane constraint guides.

## 7. Contributing

Contributions and focused bug reports are welcome.

- Open pull requests against `main` unless a dedicated development branch is announced.
- Follow Unreal Engine C++ naming and formatting conventions.
- Keep commits focused and use clear English commit messages.
- Include the tested Unreal Engine version in bug reports and pull requests.
- Describe the editor context involved: Level Editor actor, Blueprint component, Sequencer, or Control Rig.
- For transform bugs, include the coordinate space, constraint axis, snapping state, and whether the operation was confirmed or cancelled.

## 8. Compatibility

- **Target engine versions:** Unreal Engine 5.7 or newer
- **Verified:** Unreal Engine 5.8.3, Win64 — plugin compilation and editor module loading
- **Platform:** Windows is currently the supported and tested build platform
- macOS and Linux are not currently verified

## 9. License and attribution

`3DMaxEditorControlsUE5` is developed and maintained by **Figix**.

This project is a derivative of [jefimh/BlenderEditorControlsUE5](https://github.com/jefimh/BlenderEditorControlsUE5), originally created by Axiom Toolworks (`jefimh`). The upstream copyright and license terms are retained in [LICENSE](LICENSE), together with the Figix modification notice.

In summary:

- The plugin may be used and modified internally for non-commercial or commercial projects.
- Redistributed copies or modified versions must be distributed entirely free of charge.
- Redistribution must credit `jefimh` and link to the original repository.
- The plugin or a modified version may not be repackaged, sublicensed, or resold as a standalone tool, plugin, or marketplace asset.

Read [LICENSE](LICENSE) for the controlling terms.
