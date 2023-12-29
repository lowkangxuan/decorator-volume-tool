# Decorator Volume tool
A "volumetric" tool made in Unreal Engine 5 using C++ that simplifies the placement of decorations, foliage, props, etc.

Trello: https://trello.com/b/lAVw5H68/unreal-prop-projector-tool

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
You get the option to install an example project thaat includes the plugin itself, or you can choose to install only the plugin which you can then use in any of your other unreal projects.

## Adding the plugin to your project
After downloading the plugin, head to your project directory (where your `.uproject` file located), create a folder named `Plugins` (if it does not exist already), and place the tool's plugin folder in there.

Once done, open your project and open the plugins window (`Edit -> Plugins`). Under the `Installed` section, select `Projector Tool`, enable the plugin by ensuring the checkbox is checked, and restart the project.

When the above steps are done properly, the tool should be usable in your project.

# Usage
To make use of the tool itself, you will have to place a `Decorator Volume` actor into your level, assign a `Decorator Palette` to the volume actor and tweak the parameters available to your ideal look and feel.

## Creating a volume and palette
Right click the content browser, go to the "Decorator Volume" category, and you can choose to create either a `Volume` or `Palette`<br>
![](https://i.imgur.com/O9vLccu.png)
