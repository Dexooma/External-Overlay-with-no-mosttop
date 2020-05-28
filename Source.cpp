#pragma once
#include "imports.h"

void drawing() {
	Overlay* overlay{};
	overlay->window_d3x_setup();
	d3colors d3colors;

	while (true)
	{
		if (!overlay->rendering())
			return;

		overlay->Begin();
		overlay->draw_text("Example drawing text",100, 30, d3colors.Red, false, true);
		overlay->DrawBox(400, 500, 200, 250, d3colors.Red);
		overlay->End();
		if (GetAsyncKeyState(VK_END) & 1)
			ExitProcess(1);

		Sleep(25);
	}
}

int main(void) {
	drawing();
	return 0;
}