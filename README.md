# Decorator Volume tool
A "volumetric" tool made in Unreal Engine 5 using C++ that simplifies the placement of decorations, foliage, props, etc.

Roadmap: https://trello.com/b/lAVw5H68/unreal-prop-projector-tool

# Version Support
The tool itself was first developed in UE5.2 and later upgraded to UE5.3. Due to this, the tool have not been tested in both UE5.0 and 5.1, as well as any versions of UE4, so please beware when using the tool in those untested versions.

If you are interested in testing the tool out in the above mentioned versions, please head [here](https://github.com/lowkangxuan/decorator-volume-tool/issues/1) for more information.

## Legend
🟢 - Supported <br>
🟠 - WIP <br>
🔴 - Deprecated <br>
🔵 - Untested <br>

| Engine Version  | Support       |
| --------------- | ------------  |
| 4.x             | 🔵            |
| 5.0             | 🔵            |
| 5.1             | 🔵            |
| 5.2             | 🟢            |
| 5.3             | 🟢            |

# Installation
Head to the [release page](https://github.com/lowkangxuan/decorator-volume-tool/releases) to install the latest version of the tool.
You get the option to install an example project that includes the plugin itself, or you can choose to install only the plugin which you can then use in any of your other unreal projects.

## Adding the plugin to your project
After downloading the plugin, head to your project directory (where your `.uproject` file located), create a folder named `Plugins` (if it does not already exist), and place the tool's plugin folder in there.

Once done, open your project and open the plugins window (`Edit -> Plugins`). Under the `Installed` section, select `Projector Tool`, enable the plugin by ensuring the checkbox is checked, and restart the project.

When the above steps are done properly, the tool should be usable in your project.

# Usage
To make use of the tool itself, you will have to place a `Decorator Volume` actor into your level, assign a `Decorator Palette` to the volume actor and tweak the parameters available to your ideal look and feel.

There are 2 types of volumes, `Decorator Volume ISM` and `Decorator Volume HISM`. Both have similar functionality, except the `HISM` version uses Hierarchical Instanced Static Mesh Component to store the instances which allows for mesh LODs.

## Creating a volume and palette
Right click the content browser, go to the "Projector Tool" category, choose the asset to create.<br><br>
![](https://i.imgur.com/hA83q8W.png)

## Baking and Unbaking
A baking and unbaking feature for each volume that allows editing each individual instance Transform. 

### Instance Proxy Mesh
The unbaked instances are essentially just an `Instance Proxy Mesh` that derives from an actor and has only a static mesh component tagged to it. The proxy meshes are actually set to be hidden in game to prevent for major performance loss if you were to unbake a volume with many instances.

Holding the `S` hotkey while moving the proxy mesh allows itself to snap to the nearest surface and orientates to the normal of the snapped surface.
