const std = @import("std");

const CXXStandard = enum {
    cpp11,
    cpp14,
    cpp17,
    cpp20,
    cpp23,

    pub fn toFlag(self: CXXStandard) []const u8 {
        return switch (self) {
            .cpp11 => "-std=c++11",
            .cpp14 => "-std=c++14",
            .cpp17 => "-std=c++17",
            .cpp20 => "-std=c++20",
            .cpp23 => "-std=c++23",
        };
    }
};

// Although this function looks imperative, note that its job is to
// declaratively construct a build graph that will be executed by an external
// runner.
pub fn build(b: *std.Build) void {
    // Standard target options allows the person running `zig build` to choose
    // what target to build for. Here we do not override the defaults, which
    // means any target is allowed, and the default is native. Other options
    // for restricting supported target set are available.
    const target = b.standardTargetOptions(.{});

    // Standard optimization options allow the person running `zig build` to select
    // between Debug, ReleaseSafe, ReleaseFast, and ReleaseSmall. Here we do not
    // set a preferred release mode, allowing the user to decide how to optimize.
    const optimize = b.standardOptimizeOption(.{});

    const cxx_std: CXXStandard = b.option(
        CXXStandard,
        "std",
        "C++ Version To Use",
    ) orelse .cpp11;

    const fltk = b.dependency("fltk", .{});
    const modfltk = b.createModule(.{
        .target = target,
        .optimize = optimize,
        .link_libc = true,
        .link_libcpp = true,
    });
    modfltk.addCMacro("FL_LIBRARY", "");

    modfltk.addCSourceFiles(.{
        .files = &.{
            "src/Fl.cxx",
            "src/Fl_Adjuster.cxx",
            "src/Fl_Bitmap.cxx",
            "src/Fl_Browser.cxx",
            "src/Fl_Browser_.cxx",
            "src/Fl_Browser_load.cxx",
            "src/Fl_Box.cxx",
            "src/Fl_Button.cxx",
            "src/Fl_Chart.cxx",
            "src/Fl_Check_Browser.cxx",
            "src/Fl_Check_Button.cxx",
            "src/Fl_Choice.cxx",
            "src/Fl_Clock.cxx",
            "src/Fl_Color_Chooser.cxx",
            "src/Fl_Copy_Surface.cxx",
            "src/Fl_Counter.cxx",
            "src/Fl_Device.cxx",
            "src/Fl_Dial.cxx",
            "src/Fl_Double_Window.cxx",
            "src/Fl_File_Browser.cxx",
            "src/Fl_File_Chooser.cxx",
            "src/Fl_File_Chooser2.cxx",
            "src/Fl_File_Icon.cxx",
            "src/Fl_File_Input.cxx",
            "src/Fl_Flex.cxx",
            "src/Fl_Graphics_Driver.cxx",
            "src/Fl_Grid.cxx",
            "src/Fl_Group.cxx",
            "src/Fl_Help_View.cxx",
            "src/Fl_Image.cxx",
            "src/Fl_Image_Surface.cxx",
            "src/Fl_Input.cxx",
            "src/Fl_Input_.cxx",
            "src/Fl_Input_Choice.cxx",
            "src/Fl_Light_Button.cxx",
            "src/Fl_Menu.cxx",
            "src/Fl_Menu_.cxx",
            "src/Fl_Menu_Bar.cxx",
            "src/Fl_Menu_Button.cxx",
            "src/Fl_Menu_Window.cxx",
            "src/Fl_Menu_add.cxx",
            "src/Fl_Menu_global.cxx",
            "src/Fl_Message.cxx",
            "src/Fl_Multi_Label.cxx",
            "src/Fl_Native_File_Chooser.cxx",
            "src/Fl_Overlay_Window.cxx",
            "src/Fl_Pack.cxx",
            "src/Fl_Paged_Device.cxx",
            "src/Fl_Pixmap.cxx",
            "src/Fl_Positioner.cxx",
            "src/Fl_Preferences.cxx",
            "src/Fl_Printer.cxx",
            "src/Fl_Progress.cxx",
            "src/Fl_Repeat_Button.cxx",
            "src/Fl_Return_Button.cxx",
            "src/Fl_Roller.cxx",
            "src/Fl_Round_Button.cxx",
            "src/Fl_Scheme.cxx",
            "src/Fl_Scheme_Choice.cxx",
            "src/Fl_Screen_Driver.cxx",
            "src/Fl_Scroll.cxx",
            "src/Fl_Scrollbar.cxx",
            "src/Fl_Shared_Image.cxx",
            "src/Fl_Shortcut_Button.cxx",
            "src/Fl_Single_Window.cxx",
            "src/Fl_Slider.cxx",
            "src/Fl_Spinner.cxx",
            "src/Fl_Sys_Menu_Bar.cxx",
            "src/Fl_System_Driver.cxx",
            "src/Fl_Table.cxx",
            "src/Fl_Table_Row.cxx",
            "src/Fl_Tabs.cxx",
            "src/Fl_Terminal.cxx",
            "src/Fl_Text_Buffer.cxx",
            "src/Fl_Text_Display.cxx",
            "src/Fl_Text_Editor.cxx",
            "src/Fl_Tile.cxx",
            "src/Fl_Tiled_Image.cxx",
            "src/Fl_Timeout.cxx",
            "src/Fl_Tooltip.cxx",
            "src/Fl_Tree.cxx",
            "src/Fl_Tree_Item_Array.cxx",
            "src/Fl_Tree_Item.cxx",
            "src/Fl_Tree_Prefs.cxx",
            "src/Fl_Valuator.cxx",
            "src/Fl_Value_Input.cxx",
            "src/Fl_Value_Output.cxx",
            "src/Fl_Value_Slider.cxx",
            "src/Fl_Widget.cxx",
            "src/Fl_Widget_Surface.cxx",
            "src/Fl_Window.cxx",
            "src/Fl_Window_Driver.cxx",
            "src/Fl_Window_fullscreen.cxx",
            "src/Fl_Window_hotspot.cxx",
            "src/Fl_Window_iconize.cxx",
            "src/Fl_Wizard.cxx",
            "src/Fl_XBM_Image.cxx",
            "src/Fl_XPM_Image.cxx",
            "src/Fl_abort.cxx",
            "src/Fl_add_idle.cxx",
            "src/Fl_arg.cxx",
            "src/Fl_compose.cxx",
            "src/Fl_display.cxx",
            "src/Fl_get_system_colors.cxx",
            "src/Fl_grab.cxx",
            "src/Fl_lock.cxx",
            "src/Fl_own_colormap.cxx",
            "src/Fl_visual.cxx",
            "src/filename_absolute.cxx",
            "src/filename_expand.cxx",
            "src/filename_ext.cxx",
            "src/filename_isdir.cxx",
            "src/filename_list.cxx",
            "src/filename_match.cxx",
            "src/filename_setext.cxx",
            "src/fl_arc.cxx",
            "src/fl_ask.cxx",
            "src/fl_boxtype.cxx",
            "src/fl_color.cxx",
            "src/fl_contrast.cxx",
            "src/fl_cursor.cxx",
            "src/fl_curve.cxx",
            "src/fl_diamond_box.cxx",
            "src/fl_draw.cxx",
            "src/fl_draw_arrow.cxx",
            "src/fl_draw_pixmap.cxx",
            "src/fl_encoding_latin1.cxx",
            "src/fl_encoding_mac_roman.cxx",
            "src/fl_engraved_label.cxx",
            "src/fl_file_dir.cxx",
            "src/fl_font.cxx",
            "src/fl_gleam.cxx",
            "src/fl_gtk.cxx",
            "src/fl_labeltype.cxx",
            "src/fl_open_uri.cxx",
            "src/fl_oval_box.cxx",
            "src/fl_overlay.cxx",
            "src/fl_oxy.cxx",
            "src/fl_plastic.cxx",
            "src/fl_read_image.cxx",
            "src/fl_rect.cxx",
            "src/fl_round_box.cxx",
            "src/fl_rounded_box.cxx",
            "src/fl_set_font.cxx",
            "src/fl_scroll_area.cxx",
            "src/fl_shadow_box.cxx",
            "src/fl_shortcut.cxx",
            "src/fl_show_colormap.cxx",
            "src/fl_string_functions.cxx",
            "src/fl_symbols.cxx",
            "src/fl_utf8.cxx",
            "src/fl_vertex.cxx",
            "src/print_button.cxx",
            "src/screen_xywh.cxx",
        },
        .flags = &.{
            cxx_std.toFlag(),
        },
        .language = .cpp,
        .root = fltk.path(""),
    });
    modfltk.addIncludePath(fltk.path("FL"));

    const libfltk = b.addLibrary(.{
        .linkage = .static,
        .name = "fltk",
        .root_module = modfltk,
    });
    b.installArtifact(libfltk);

    // We will also create a module for our other entry point, 'main.zig'.
    const modsimba = b.createModule(.{
        .target = target,
        .optimize = optimize,
        .link_libc = true,
        .link_libcpp = true,
    });
    modsimba.linkLibrary(libfltk);
    modsimba.addCSourceFiles(.{
        .files = &.{
            "src/main.cpp",
        },
        .flags = &.{
            cxx_std.toFlag(),
        },
        .language = .cpp,
    });

    // This creates another `std.Build.Step.Compile`, but this one builds an executable
    // rather than a static library.
    const simba = b.addExecutable(.{
        .name = "simba",
        .root_module = modsimba,
    });

    // This declares intent for the executable to be installed into the
    // standard location when the user invokes the "install" step (the default
    // step when running `zig build`).
    b.installArtifact(simba);

    // This *creates* a Run step in the build graph, to be executed when another
    // step is evaluated that depends on it. The next line below will establish
    // such a dependency.
    const run_cmd = b.addRunArtifact(simba);

    // By making the run step depend on the install step, it will be run from the
    // installation directory rather than directly from within the cache directory.
    // This is not necessary, however, if the application depends on other installed
    // files, this ensures they will be present and in the expected location.
    run_cmd.step.dependOn(b.getInstallStep());

    // This allows the user to pass arguments to the application in the build
    // command itself, like this: `zig build run -- arg1 arg2 etc`
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    // This creates a build step. It will be visible in the `zig build --help` menu,
    // and can be selected like this: `zig build run`
    // This will evaluate the `run` step rather than the default, which is "install".
    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);
}

// fn flCreateModule(b: *std.Build, comptime name: []const u8) void {}
