# CronusHitstop

Cronus Hitstop is a C++ plugin that allows you to quickly configure hitstops for any actions: just plug in the affected actors, vfx, and define the style of the hitstop - the system will take care of the rest.



The system will track all active hitstops and automatically prioritize more important hitstops. With the asynchronous hitstop node, you can also run custom logic in response to hitstops being completed or cancelled early.



You can enrich your hitstop effects by adding as many cosmetic effects to your actors as you want - Cronus will take care of starting, managing, and finishing them during the hitstop. The plug in comes with 5 preconfigured effect classes (character mesh shake, damage material blink, audio ducking, dynamic camera shake, cosmetic container), and 3 generic classes so that you can add anything you want during a hitstop. Moreover, you can define custom rules for each actor by adding the hitstop interface to their blueprint.

-------------------------------------------------------


Cronus Hitstop 2.0 New Features:

- Added support for user-defined curves: control the intensity of cosmetics over time, increase/decrease the power of an effect based on camera distance, and vary the update rate of the effect dynamically.
- Noise: Mesh Shake can receive random noise on top of the shake for more organic behavior
- Frame/Time compatibility: all effects that update periodically can now be updated on frame or time intervals. The interval can also be changed throughout the hitstop to be shorter/longer dynamically.
- Slice Attacks: with certain attacks (swords, axes, anything that slices) we don't want to fully stop the actor, so we can still convey the feeling of gradually slicing something. An option to slow down targets instead of freezing them has been added.
- Cosmetic Container: sometimes we want to add many of effects at once - we can now template containers rather than configuring complex hitstops manually each time.
- Camera Shake Cosmetic: a new cosmetic made specifically to control camera shakes during hitstops has been added.
- Editor Integration: to speed up the creation process, Cronus now has its own menu entry in the Unreal Class Browser.
- Custom rules for Mesh Cosmetics: Check if in Air, and Fallback Mesh Compatibility.
