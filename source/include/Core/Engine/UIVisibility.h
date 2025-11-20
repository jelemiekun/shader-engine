#pragma once

struct UIVisibility {
  // TODO: Sync imgui::begin window name to variable names
  unsigned int left_panel : 1;
  unsigned int render_buffer : 1;
  unsigned int script_editor : 1;
  unsigned int right_panel : 1;
  unsigned int right_panel_2 : 1;
  unsigned int bottom_panel : 1;
};
