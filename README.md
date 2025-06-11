# CronusHitstop
---------------------------------
#### Cronus Hitstop is a C++ plugin that allows you to quickly configure hitstops for any actions: just plug in the affected actors, vfx, and define the style of the hitstop - the system will take care of the rest.
-------------------------------------
[Feature Showcase 1.0](https://www.youtube.com/watch?v=UpVrzt5G91E)

[Feature Showcase 2.0](https://www.youtube.com/watch?v=HZRNNJpWQns)

The system will track all active hitstops and automatically prioritize more important hitstops. With the asynchronous hitstop node, you can also run custom logic in response to hitstops being completed or cancelled early.
You can enrich your hitstop effects by adding as many cosmetic effects to your actors as you want - Cronus will take care of starting, managing, and finishing them during the hitstop. The plug in comes with 5 preconfigured effect classes (character mesh shake, damage material blink, audio ducking, dynamic camera shake, cosmetic container), and 3 generic classes so that you can add anything you want during a hitstop. Moreover, you can define custom rules for each actor by adding the hitstop interface to their blueprint.

-------------------------------------------------------

#### Cronus Hitstop 2.0 New Features:

- Added support for user-defined curves: control the intensity of cosmetics over time, increase/decrease the power of an effect based on camera distance, and vary the update rate of the effect dynamically.
- Noise: Mesh Shake can receive random noise on top of the shake for more organic behavior
- Frame/Time compatibility: all effects that update periodically can now be updated on frame or time intervals. The interval can also be changed throughout the hitstop to be shorter/longer dynamically.
- Slice Attacks: with certain attacks (swords, axes, anything that slices) we don't want to fully stop the actor, so we can still convey the feeling of gradually slicing something. An option to slow down targets instead of freezing them has been added.
- Cosmetic Container: sometimes we want to add many of effects at once - we can now template containers rather than configuring complex hitstops manually each time.
- Camera Shake Cosmetic: a new cosmetic made specifically to control camera shakes during hitstops has been added.
- Editor Integration: to speed up the creation process, Cronus now has its own menu entry in the Unreal Class Browser.
- Custom rules for Mesh Cosmetics: Check if in Air, and Fallback Mesh Compatibility.
  
---------------------------------------------------

### Table of Contents
- [Creating Your First Hitstop](#creating-your-first-hitstop).
- [Other Types of Hitstops & Helpful Nodes](#other-types-of-hitstop).
- [Creating Custom Hitstop Cosmetics](#creating-custom-hitstop-cosmetics).
- [Creating Custom Hitstop Effects](#creating-custom-hitstop-effects).
- [Defining Custom Rules](#defining-custom-rules).

#### Creating Your First Hitstop
1.	To create a hitstop, go to the blueprint responsible for the hit event. Depending on your project, this could be a gameplay ability, a character, or an actor component.

2.	Type “hitstop” in the content browser and you will see a few options. (Function names may vary as the plugin gets updated)
 ![image](https://github.com/user-attachments/assets/8495147a-000e-48a6-867b-08a2977c9b78)

3.	Select `Get HitStopSubsystem` drag out of the subsystem node, and choose `Simple Hitstop`. 
![image](https://github.com/user-attachments/assets/a36f67be-5bb4-4797-a130-4de283d3ef08)

4.	There are two types of hitstops:_ time based_, and _frame based_. Choose a type in the node according to your preferences. Both types can be used simultaneously.

5.	Drag out of the Targets array, and type MakeArray
![image](https://github.com/user-attachments/assets/78bcc366-efa5-4923-a33c-e1bea652e1b1)

6.	Right click the 0 pin on the MakeArraynode to expand it. You can now fill the inputs with your hitstopped actors and their related Niagara systems that should be hitstopped.
 ![image](https://github.com/user-attachments/assets/642b10c5-4590-429f-be94-6388890ddf64)

7.	Modify the `Duration` value in the `SimpleHitstop` node to define the duration of the hitstop in seconds.

8.	Plug in the Simple Hitstop node into the rest of your attack/hit/damage function, and your first hitstop setup will be complete.

9.	Now try it out yourself: your target actors (the actor inducing the hitstop, and the actors receiving the hitstop) and their impact VFX will now freeze for the duration of the Simple Hitstop node.
    
---

#### Other Types of Hitstop
Now that you have set up a simple hitstop, it’s time to look at more advanced functionality.
`Call HitStop and Wait` is an asynchronous node. It works just like `SimpleHitstop`, but it allows you to run logic when a hitstop is completed (OnComplete) and when a hitstop is cancelled (OnCancelled).  The `OnCancelled` pin will fire when the hitstop is cancelled in favor of a new hitstop.
![image](https://github.com/user-attachments/assets/9dbc9d76-d9b6-4d65-b8ca-15c4b5f8b07a)

`Cancel All Active Hitstops` will cancel all the hitstops currently running in your game (and consequently, trigger the `OnCancelled` pin of any active Call HitStop and Wait nodes).
![image](https://github.com/user-attachments/assets/f6a4e1ba-b955-4752-a0cf-8041745493b1)


`MakeSimpleHitstopData` allows you to define hitstop data while keeping your nodes slightly more compact. Its functionality is analogous to the `MakeArray` and `SplitStructPin` nodes described earlier.
![image](https://github.com/user-attachments/assets/5d50e671-ef3c-452b-8487-37539d23004a)

---

#### Creating Custom Hitstop Cosmetics
Cronus lets you define custom behavior for your hitstopped actors in the form of Hitstop Cosmetics. 

Hitstop Cosmetics can be added to a hitstop from the Cosmetic section of the `Make Simple Hit Stop Data` node and in the generic MakeArray of structs target that is input into the Hitstop nodes.

Creating a new **Hitstop Cosmetic**
-	From your content browser right click as if you were to create any other class, and look for the “Hitstop” menu entry. 
-	Hover over the entry and click on the Hitstop Cosmetic class option.
![image](https://github.com/user-attachments/assets/c2ee3657-5ace-4a3e-884b-80eeaa498a57)

-	A pop up menu will prompt you to choose a parent class for your new hitstop cosmetic.
![image](https://github.com/user-attachments/assets/072fbd5c-7f07-48d4-aa79-28bec528212f)
-	Pick any of the available classes and choose ok to create a new Hitstop Cosmetic Blueprint.
-	Note: for some classes, the ok button is hidden. That’s because they are abstract c++ classes not meant to be modified, and you should modify their available subclasses instead.

Overview

There are 5 main cosmetic types, each with their own set of features and degrees of complexity. Here follows a breakdown of all the functionality of one of the most complex cosmetics – the **Hit Stop Mesh Shake**. Read the tool tips on each cosmetic variable from the Editor to learn more.
![image](https://github.com/user-attachments/assets/902fa949-08eb-4472-8a1a-3fe7fdf3e70c)

`Shake Amount`: This determines the base amount that the target mesh will shake in each direction, in relative space. If any direction is left at 0, no shake will be applied (also for additional modifiers, more on that in the next points).

`Noise`: This adds the chosen amount of Noise in Unreal Units to the shake. It adds some randomization to the effect shake amount that makes it more organic and appealing.

`Only Shake Z Axis When In Air`: You may only want to shake your target’s mesh vertically if they are airborne.  The cosmetic can automatically detect if subclasses of Pawns or Characters are in the air.

`Is Affected By Distance`: Checking this variable will enable the Distance Intensity Curve. This curve lets you modify how strong the shake will be according to how far the target is from the camera. You might want to apply very little shake when the camera is really close to the target, and progressively apply much more so that it can be exaggerated when the camera is really far. 

`Use Fallback Mesh`: Sometimes you want to Hitstop actors that aren’t characters and have multiple skeletal meshes in their hierarchy. To pick the right root mesh to add shake to, you can add a Name Tag to it and add the same tag under Fallback Mesh Tag.

`Intensity Curve`: This curve controls how intense the shake is over the duration of the Hitstop. For example, you could have a lot of shake at the start of the Hitstop, and gradually decrease its intensity over time. The duration of the curve is always between 0 and 1, so you don’t have to worry about making it match the real time/frame duration of your Hitstops.

`Update Type`: The cosmetic can be updated over time using game frames or game time. Pick whichever suits your design best.

`Frequency`: How often (how many seconds/frames) between each update of the effect.

`Update Curve`: Changes the frequency of the update over the duration of the Hitstop. Useful to make a cosmetic update very fast when the target is hit, and slow down as the Hitstop fades out.


Cronus comes with 5 predefined Cosmetics:
-	`Mesh Shake`: shakes the character mesh during the hitstop.
-	`Material Animation`: adds an overlay material during a hitstop and makes it blink. 
-	`Filter Audio`: allows modifications of audio mixes during a hitstop (such as temporarily muting an audio type during a hit)
-	`Camera Shake`: Shakes the camera for the duration of the hitstop.
-	`Cosmetic Container`: Holds multiple cosmetics in one place so that they can be templated and applied at once.

Each of these cosmetics have variables that can be configured by designers to match the specific needs of their project.

You can see some examples of subclasses of these effects by going to Plugins->CronusHitstopContent->Cosmetics. Apply any of these effects during a hitstop to see the results.
![image](https://github.com/user-attachments/assets/f16e4a6f-1ff4-4af3-84fa-e60aba2b6312)


#### Creating Custom Hitstop Effects

If you want to create entirely new effects, then select Histop_Cosmetic as your base class.

Hitstop_Cosmetic comes with 3 predefined events available from the action menu:
![image](https://github.com/user-attachments/assets/c735e164-153e-4b6b-8f76-3429d499f93b)


for instance, you wanted to implement a cosmetic effect that adds a camera shake during a hitstop:

`On Effect Started`, you would start the camera shake.

`On Effect Ended`, you would stop the shake.

`On Effect Updated` is the event that fires X times per second, according to the `frequency` variable (available from the Class Defaults panel). In the case of a `camera shake`, it does not need to be implemented.

---

#### Defining Custom Rules

**The Hitstop Interface**

There are times when you don’t want an actor to be hitstopped. This could be due to a range of reasons – maybe you don’t want to interfere with a “Game Over” animation, or you don’t want to hitstop characters that are getting back up after being knocked to the ground. 

In cases like these, you can add the `Hitstop Interface` to your actor blueprint. Go to your actor blueprint’s Class Settings panel, and Add the Hitstop Interface to the Implemented Interfaces.
on the left side of your blueprint, under the MyBlueprint->Interfaces panel. You will see 3 functions that can be configured with your custom hitstop related logic. 

For instance, adding a bool condition to the `Can Be Hit Stopped` function will decide whether the actor can be hitstopped or not.

---

If you have any questions or would like to share your hitstop creations, please reach out :)
