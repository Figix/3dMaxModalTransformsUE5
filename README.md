# 3ds Max Editor Controls — Phase 1 MVP

An editor-only Unreal Engine plugin that starts true modal transforms from the keyboard:

- `W`: translate
- `E`: rotate
- `R`: scale
- `X`, `Y`, `Z`: axis constraint
- `Shift + X/Y/Z`: exclude that axis (plane constraint)
- numeric keys: exact value input
- `Left Mouse Button`, `Enter`, or `Space`: confirm
- `Right Mouse Button` or `Escape`: cancel and restore the initial transform

The transform continues from mouse movement without holding a mouse button. Input starts only while the cursor is over an editor viewport, and is ignored while typing, while a menu is open, or during PIE.

## Phase 1 scope

The fork retains the extensible session/tool/pivot separation of the source project. Actor transforms, multi-selection, Blueprint component pivots, Control Rig pivots, snapping, numeric input, and transaction support are present in the inherited implementation; the project acceptance target for this phase is the single-Actor Level Editor workflow.

## Coexistence

`BlenderEditorControlsUE5` is not modified. Both plugins can remain installed. When both are enabled, this plugin registers at a slightly higher input-preprocessor priority, so `R` starts 3ds Max-style Scale instead of the Blender plugin's Rotate. Disable either plugin when you want its complete, unambiguous shortcut set.

## Attribution and license

3DMaxEditorControlsUE5 is developed and maintained by **Figix**.

This plugin is a derivative of [jefimh/BlenderEditorControlsUE5](https://github.com/jefimh/BlenderEditorControlsUE5), originally created by Axiom Toolworks (`jefimh`). The upstream license is retained in `LICENSE` together with the Figix modification notice.

Redistribution must comply with the upstream terms: redistributed copies must remain entirely free, credit `jefimh`, and link to the original repository. The plugin or a modified version may not be resold as a standalone tool, plugin, or asset.
