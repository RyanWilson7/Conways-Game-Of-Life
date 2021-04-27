#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <string>
#include <vector>
#define ALIVE 1
enum class Pattern { random,
    gun,
    infinite,
    pentomino };

struct Button {
    int x;
    int y;
    int w;
    int h;
    std::string text;
    bool enabled;
    Pattern p;
};

class CGameOfLife : public olc::PixelGameEngine {
public:
    Pattern selectedPattern = Pattern::random;
    std::vector<Button> buttons;
    int* output;
    int* state;
    int size;

    CGameOfLife() { sAppName = "CGameOfLife"; }

    void ResetWithPattern(Pattern p)
    {
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
        switch (p) {
        case Pattern::random:
            for (int i = 0; i < size; i++)
                state[i] = rand() % 2;
            break;
        case Pattern::gun:
            // Gosper Glider Gun
            set(60, 45, L"........................#............");
            set(60, 46, L"......................#.#............");
            set(60, 47, L"............##......##............##.");
            set(60, 48, L"...........#...#....##............##.");
            set(60, 49, L"##........#.....#...##...............");
            set(60, 50, L"##........#...#.##....#.#............");
            set(60, 51, L"..........#.....#.......#............");
            set(60, 52, L"...........#...#.....................");
            set(60, 53, L"............##.......................");
            break;
        case Pattern::infinite:
            // Infinite Growth
            set(100, ScreenHeight() / 2, L"########.#####...###......#######.#####");
            break;
        case Pattern::pentomino:
            // R-Pentomino
            set(ScreenWidth() / 2, ScreenHeight() / 2, L"  ## ");
            set(ScreenWidth() / 2, ScreenHeight() / 2 + 1, L" ##  ");
            set(ScreenWidth() / 2, ScreenHeight() / 2 + 2, L"  #  ");
            break;
        }
    }
    bool OnUserCreate() override
    {
        int width = 80;
        int height = 20;
        buttons.push_back({ 0, ScreenHeight() - height, width, height, "Random",
            true, Pattern::random });
        buttons.push_back({ width, ScreenHeight() - height, width, height, "Gun",
            false, Pattern::gun });
        buttons.push_back({ width * 2, ScreenHeight() - height, width, height,
            "Infinite", false, Pattern::infinite });
        buttons.push_back({ width * 3, ScreenHeight() - height, width, height,
            "Pentomino", false, Pattern::pentomino });
        ResetWithPattern(Pattern::random);
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        Clear(olc::BLACK);
        auto cell = [&](int x, int y) { return output[y * ScreenWidth() + x]; };
        auto IsPointInButton = [&](int x, int y, Button b) {
            return (x > b.x && x < b.x + b.w && y > b.y && y < b.y + b.h);
        };
        auto mousePos = GetMousePos();

        if (GetMouse(false).bPressed) {
            auto mousePos = GetMousePos();
            for (auto& b : buttons) {
                bool clicked = IsPointInButton(mousePos.x, mousePos.y, b);
                if (clicked && !b.enabled) {
                    // disable all buttons
                    for (auto& d : buttons)
                        d.enabled = false;
                    // enable the clicked button
                    b.enabled = true;
                    // reset to the new pattern
                    ResetWithPattern(b.p);
                    break;
                }
            }
        }
        for (int i = 0; i < size; i++)
            output[i] = state[i];

        for (int x = 0; x < ScreenWidth(); x++)
            for (int y = 0; y < ScreenHeight(); y++) {
                int neighbors = cell(x - 1, y - 1) + cell(x, y - 1) + cell(x + 1, y - 1);
                neighbors += cell(x - 1, y) + cell(x + 1, y);
                neighbors += cell(x - 1, y + 1) + cell(x, y + 1) + cell(x + 1, y + 1);

                if (cell(x, y) == ALIVE) {
                    state[y * ScreenWidth() + x] = neighbors == 2 || neighbors == 3;
                    Draw(x, y, olc::WHITE);
                } else {
                    state[y * ScreenWidth() + x] = neighbors == 3;
                }
            }

        for (auto& b : buttons) {
            FillRect(b.x, b.y, b.w, b.h, b.enabled ? olc::GREEN : olc::RED);
            DrawString(b.x, b.y, b.text);
        }
        return true;
    }

    bool OnUserDestroy() override
    {
        delete[] output;
        delete[] state;
        return true;
    }
};
int main()
{
    CGameOfLife life;
    if (life.Construct(640, 480, 4, 4))
        life.Start();
    return 0;
}
