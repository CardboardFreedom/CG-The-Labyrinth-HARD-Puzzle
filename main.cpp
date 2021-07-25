#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <tuple>
#include <list>

using namespace std;

class dot;
class position;

list<position> path;

bool solution;

int R; // number of rows.
int C; // number of columns.
int A; // number of rounds between the time the alarm countdown is activated and the time the alarm goes off.

int xC = -1, yC; // Control Room Coordinates
int xT = -1, yT; // T Coordinates

int questions = 0, dots = 0;

string spike = "";


class position
{
public:

    position() {}

    position(int a, int b)
    {
        x = a;
        y = b;
    }

    int x, y;
};


class dot
{
public:
    char contains = '/';
    dot* parent = nullptr;
    position coord;

    dot() {}

    dot(char s, position c)
    {
        contains = s;
        coord = c;
    }
};

void search(vector<vector<dot>> masterMaze, position sP, char s)
{
    list<dot*> queue;

    cerr << "ENTERED SEARCH:\n";

    queue.push_back(&masterMaze[sP.x][sP.y]);

    while (!queue.empty())
    {
        //cerr << "LOOPING!\n\n";
        // Dequeue a vertex from queue and print it
        auto check = queue.front();
        if (check->contains == s)
        {
            cerr << "WE FOUND A SOLUTION!\n" <<
                check->coord.x << ' ' << check->coord.y << ' ' << check->contains << "\n\n";

            solution = true;

            path.push_front(check->coord);

            while (check->parent != nullptr)
            {
             //   cerr << check->coord.x << ' ' << check->coord.y << "  " << check->parent->coord.x <<
             //       ' ' << check->parent->coord.y << '\n';

             //   cerr << check << ' ' << check->parent << '\n';

                check = check->parent;

                if(s == 'C' && check->coord.x == xT && check->coord.y == yT)
                    return;

                if(s == 'T' && check->coord.x == xC && check->coord.y == yC)
                    return;

                for (auto& i : path)
                {
                    if (i.x == check->coord.x && i.y == check->coord.y)
                        return;
                }

                path.push_front(check->coord);
            }

            return;
        }
        queue.pop_front();

        if (check->coord.x + 1 < R && masterMaze[check->coord.x + 1][check->coord.y].parent == nullptr &&
            masterMaze[check->coord.x + 1][check->coord.y].contains != '#' &&
            masterMaze[check->coord.x + 1][check->coord.y].contains != '?')
        {
            masterMaze[check->coord.x + 1][check->coord.y].parent = check;
            queue.push_back(&masterMaze[check->coord.x + 1][check->coord.y]);
        }

        if (check->coord.x - 1 >= 0 && masterMaze[check->coord.x - 1][check->coord.y].parent == nullptr &&
            masterMaze[check->coord.x - 1][check->coord.y].contains != '#' &&
            masterMaze[check->coord.x - 1][check->coord.y].contains != '?')
        {
            masterMaze[check->coord.x - 1][check->coord.y].parent = check;
            queue.push_back(&masterMaze[check->coord.x - 1][check->coord.y]);
        }

        if (check->coord.y + 1 < C && masterMaze[check->coord.x][check->coord.y + 1].parent == nullptr &&
            masterMaze[check->coord.x][check->coord.y + 1].contains != '#' &&
            masterMaze[check->coord.x][check->coord.y + 1].contains != '?')
        {
            masterMaze[check->coord.x][check->coord.y + 1].parent = check;
            queue.push_back(&masterMaze[check->coord.x][check->coord.y + 1]);
        }

        if (check->coord.y - 1 >= 0 && masterMaze[check->coord.x][check->coord.y - 1].parent == nullptr &&
            masterMaze[check->coord.x][check->coord.y - 1].contains != '#' &&
            masterMaze[check->coord.x][check->coord.y - 1].contains != '?')
        {
            masterMaze[check->coord.x][check->coord.y - 1].parent = check;
            queue.push_back(&masterMaze[check->coord.x][check->coord.y - 1]);
        }
    }
}

int main()
{
    cin >> R >> C >> A;

    vector<vector<dot>> masterMaze;
    masterMaze.resize(R);

    for (auto& i : masterMaze)
        i.resize(C);

    bool back = 0, block = 0;

    vector<string> innerPath;

    vector<pair<int, int>> prev;
    string pMove = "/", pM = "/";

    // game loop
    while (1)
    {
        int KR; // row where Kirk is located.
        int KC; // column where Kirk is located.
        int x = -1, y;
        cin >> KR >> KC; cin.ignore();
        if (xT == -1)
        {
            xT = KR;
            yT = KC;
            cerr << xT << ' ' << yT << '\n';
        }
        prev.push_back({ KC, KR });
        string out = "";
        bool up = 1, down = 1, left = 1, right = 1;

        vector<string> maze;

        for (int i = 0; i < R; i++)
        {
            string ROW;
            cin >> ROW; cin.ignore();
            maze.push_back(ROW);

            if (masterMaze[0][0].contains != '/')
                for (int j = 0; j < ROW.size(); j++)
                    if (masterMaze[i][j].contains == '?' && ROW[j] != '?')
                    {
                        if (ROW[j] == 'C') { cerr << "Found Control Room\n"; x = i; y = ROW.find('C'); xC = x; yC = y; cerr << "Coordinates: " << x << ' ' << y << '\n'; }
                        if (ROW[j] == '.')dots++;
                        questions--;
                        masterMaze[i][j].contains = ROW[j];
                        masterMaze[i][j].coord = position(i, j);
                    }

        }

        if (masterMaze[0][0].contains == '/')
        {
            for (int i = 0; i < masterMaze.size(); i++)
            {
                for (int j = 0; j < masterMaze[i].size(); j++)
                {
                    masterMaze[i][j].contains = maze[i][j];
                    masterMaze[i][j].coord = position(i, j);

                    if (maze[i][j] == '.')dots++;
                    if (maze[i][j] == '?')questions++;
                }
            }
        }

        for (auto i : masterMaze)
        {
            for (auto g : i)
                cerr << g.contains;
            cerr << '\n';
        }

        if (x == -1)
            block = 0;

        if (maze[KR][KC] == 'C')
        {
            back = 1;
        }


        if (back)
        {
            if (!solution)
            {
                path.clear();
                cerr << "BACK HERE: ";
                search(masterMaze, position(xC, yC), 'T');

                auto check = path.front();

                while(check.x == KR && check.y == KC)
                {
                    path.pop_front();
                    check = path.front();
                }
            }

            if (path.front().y > KC)
                cout << "RIGHT\n";
            else if (path.front().y < KC)
                cout << "LEFT\n";
            else if (path.front().x < KR)
                cout << "UP\n";
            else if (path.front().x > KR)
                cout << "DOWN\n";

            path.pop_front();

            continue;
        }

        if (path.size())
        {
            if (path.front().y > KC)
                cout << "RIGHT\n";
            else if (path.front().y < KC)
                cout << "LEFT\n";
            else if (path.front().x < KR)
                cout << "UP\n";
            else if (path.front().x > KR)
                cout << "DOWN\n";

            path.pop_front();

            continue;
        }



        if (KC + 1 >= C || maze[KR][KC + 1] == '#' || (maze[KR][KC + 1] == 'C' && (double(questions) / (double(R) * double(C)) * 100.0) > 5))
        {//cerr<<"RIGHT: " << maze[KR][KC+1] << ' ';
            right = 0;
        }
        if (KC - 1 < 0 || maze[KR][KC - 1] == '#' || (maze[KR][KC - 1] == 'C' && (double(questions) / (double(R) * double(C)) * 100.0) > 5))
        {//cerr<<"LEFT: "<<maze[KR][KC-1] << ' ';
            left = 0;
        }
        if (KR - 1 < 0 || maze[KR - 1][KC] == '#' || (maze[KR - 1][KC] == 'C' && (double(questions) / (double(R) * double(C)) * 100.0) > 5))
        {//cerr<<"DOWN: "<<maze[KR-1][KC] << ' ';
            up = 0;
        }
        if (KR + 1 >= R || maze[KR + 1][KC] == '#' || (maze[KR + 1][KC] == 'C' && (double(questions) / (double(R) * double(C)) * 100.0) > 5))
        {//cerr<<"UP: "<<maze[KR+1][KC] << ' ';
            down = 0;
        }

        if (up && (find(prev.begin(), prev.end(), make_pair(KC, KR - 1)) == prev.end() || maze[KR - 1][KC] == 'C'))out = "UP";
        if (pMove == out)
        {
            innerPath.push_back(out);
            cout << out << '\n';
            continue;
        }
        if (down && (find(prev.begin(), prev.end(), make_pair(KC, KR + 1)) == prev.end() || maze[KR + 1][KC] == 'C'))out = "DOWN";
        if (pMove == out)
        {
            innerPath.push_back(out);
            cout << out << '\n';
            continue;
        }
        if (left && (find(prev.begin(), prev.end(), make_pair(KC - 1, KR)) == prev.end() || maze[KR][KC - 1] == 'C'))out = "LEFT";
        if (pMove == out)
        {
            innerPath.push_back(out);
            cout << out << '\n';
            continue;
        }
        if (right && (find(prev.begin(), prev.end(), make_pair(KC + 1, KR)) == prev.end() || maze[KR][KC + 1] == 'C'))out = "RIGHT";
        if (pMove == out)
        {
            innerPath.push_back(out);
            cout << out << '\n';
            continue;
        }

        if (out.empty() && innerPath.size())
        {
            pMove = innerPath.back();
            if (pMove == "RIGHT")out = "LEFT";
            if (pMove == "LEFT")out = "RIGHT";
            if (pMove == "DOWN")out = "UP";
            if (pMove == "UP")out = "DOWN";
            cout << out << '\n';
            innerPath.pop_back();
            continue;
        }

        else if (out.empty())
        {
            cerr << "SPIKE HERE: ";
            search(masterMaze, position(KR, KC), 'C');
            solution = false;

            auto check = path.front();

            while(check.x == KR && check.y == KC)
            {
                path.pop_front();
                check = path.front();
            }

            cerr << "Check: " << KR << ' ' << KC << ' ' << check.x << ' ' << check.y << '\n';

            if (path.front().y > KC)
                cout << "RIGHT\n";
            else if (path.front().y < KC)
                cout << "LEFT\n";
            else if (path.front().x < KR)
                cout << "UP\n";
            else if (path.front().x > KR)
                cout << "DOWN\n";

            path.pop_front();

            continue;
        }

        innerPath.push_back(out);
        pMove = out;

        cout << out << '\n'; // Kirk's next move (UP DOWN LEFT or RIGHT).
    }
}
