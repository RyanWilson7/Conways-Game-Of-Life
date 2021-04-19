#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <string>
#define ALIVE 1
class CAutomata : public olc::PixelGameEngine {
public:
  int *output;
  int *state;
  int size;

  CAutomata() { sAppName = "CAutomata"; }

  bool OnUserCreate() override {
    size = ScreenWidth() * ScreenHeight();
    output = new int[size];
    state = new int[size];
    memset(output, 0, size * sizeof(int));
    memset(state, 0, size * sizeof(int));

    auto set = [&](int x, int y, std::wstring s) {
      int p = 0;
      for (auto c : s) {
        state[y * ScreenWidth() + x + p] = c == L'#' ? 1 : 0;
        p++;
      }
    };

    for (int i = 0; i < size; i++) {
      state[i] = rand() % 2;
    }

    return true;
  }

  bool OnUserUpdate(float fElapsedTime) override {

    auto cell = [&](int x, int y) { return output[y * ScreenWidth() + x]; };
    for (int i = 0; i < size; i++)
      output[i] = state[i];

    for (int x = 0; x < ScreenWidth(); x++)
      for (int y = 0; y < ScreenHeight(); y++) {
        int neighbors =
            cell(x - 1, y - 1) + cell(x, y - 1) + cell(x + 1, y - 1);
        neighbors += cell(x - 1, y) + cell(x + 1, y);
        neighbors += cell(x - 1, y + 1) + cell(x, y + 1) + cell(x + 1, y + 1);

        if (cell(x, y) == ALIVE) {
          state[y * ScreenWidth() + x] = neighbors == 2 || neighbors == 3;
          Draw(x, y, olc::WHITE);
        } else {
          state[y * ScreenWidth() + x] = neighbors == 3;
          Draw(x, y, olc::BLACK);
        }
      }

    return true;
  }

  bool OnUserDestroy() override {
    delete[] output;
    delete[] state;
    return true;
  }
};
int main() {
  CAutomata life;
  if (life.Construct(640, 480, 4, 4))
    life.Start();
  return 0;
}
