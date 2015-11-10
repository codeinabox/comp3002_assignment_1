/*  Assignment 1 - Mathew Attlee SID 9816022 */

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include <vector>
#include <set>
#include <time.h>
#include <string>

using namespace std;

// the heuristic
int dist(int n1, int n2) {
    int d1[3], d2[3];

    d1[0] = n1 / 100;
    d1[1] = n1 / 10 - d1[0] * 10;
    d1[2] = n1 - d1[0] * 100 - d1[1] * 10;

    d2[0] = n2 / 100;
    d2[1] = n2 / 10 - d2[0] * 10;
    d2[2] = n2 - d2[0] * 100 - d2[1] * 10;

    return abs(d1[0] - d2[0]) + abs(d1[1] - d2[1]) + abs(d1[2] - d2[2]);
}


class Node {
public:
    Node * parent;
    int value, h, g, f, depth, move;
    // move corresponds to 0 = none, 1 = first digit, 2 = second digit etc

    Node() {};
    Node(int v, int m, Node * p) {
        value = v;
        move = m;
        parent = p;
    };

    // node expansion rountine
    vector<Node*> expand(const set<int> & forbid) {
        int digit[3];
        vector<Node*> kids;

        // tokenize the value into single digits
        digit[0] = value / 100;
        digit[1] = value / 10 - digit[0] * 10;
        digit[2] = value - digit[0] * 100 - digit[1] * 10;

        // 1st digit
        if(move != 1) {
            if(digit[0] != 0) if(!forbid.count(value-100)) kids.push_back(new Node(value - 100, 1, this)); // subbing
            if(digit[0] != 9) if(!forbid.count(value+100)) kids.push_back(new Node(value + 100, 1, this)); // adding
        }

        // 2nd digit
        if(move != 2) {
            if(digit[1] != 0) if(!forbid.count(value-10)) kids.push_back(new Node(value - 10, 2, this)); // subbing
            if(digit[1] != 9) if(!forbid.count(value+10)) kids.push_back(new Node(value + 10, 2, this)); // adding
        }

        // 3rd digit
        if(move != 3) {
            if(digit[2] != 0) if(!forbid.count(value-1)) kids.push_back(new Node(value - 1, 3, this)); // subbing
            if(digit[2] != 9) if(!forbid.count(value+1)) kids.push_back(new Node(value + 1, 3, this)); // adding
        }

        return kids;
    }
};


void stats(int memcost, int expanded, clock_t time) {
    cout << "\nMaximum Number of Nodes in Memory: " << memcost << endl;
    cout << "Number of Expanded Nodes: " << expanded << endl;
    cout << "Time (Clock Ticks): " << time << endl;
}


/***********************
 * Bread First Search
 ***********************/
void bfs(int s, int g, const set<int> & forbid, bool verbose)
{
    Node * start, * goal, * current;
    clock_t start_time, end_time, total_time;
    int node_count = 0;
    queue<Node*> q;

    cout << "### Bread First Search ###\n\n";
    start_time = clock();

    // Create the initial state, insert it on a queue
    start = new Node(s, 0, NULL);
    goal = NULL;
    q.push(start);

    // keep searching the queue while it isn't empty, haven't exceed max nodes and haven't found the goal
    while(!q.empty() && node_count < 1000 && !goal) {
        current = q.front();
        q.pop(); // pop the node of the front of the queue
        if(current->value == g) goal = current; // check if the current is the goal
        if(verbose) cout << current->value << ", ";
        node_count++; // increase the count

        // expand the kids, push them onto the back of the queue
        vector<Node*> kids = current->expand(forbid);
        for(unsigned int c = 0; c < kids.size(); c++) {
            q.push(kids[c]);
        }
    }

    // calculate the time
    end_time = clock();
    total_time = end_time - start_time;

    cout << "\n\n### Results ###\n";

    if(goal) {
        cout << "The solution (backwards) is" << endl;
        int path_cost = 0;
        while(current) {
            cout << current->value;
            current = current->parent;
            if(current) cout << " -> ";
            path_cost++;
        }
        cout << endl << "Which has a path cost of " << path_cost;
    }
    else cout << "No solution was found" << endl;

    stats(node_count, node_count, total_time);
}

struct t_less_greedy : binary_function <Node*, Node*, bool>
{
    bool operator()(Node * n1, Node * n2)   {
        if (n1->h >= n2->h) return true;
        else return false;
    }
};

void greedy(int s, int g, const set<int> & forbid, bool verbose)
{
    Node * start, * goal, * current, * tchild;
    clock_t start_time, end_time, total_time;
    int node_count = 0;
    priority_queue<Node*, vector<Node*>, t_less_greedy> q;

    cout << "\n### Starting Greedy ###\n";
    start_time = clock();

    // Create the initial state, insert it on a queue
    goal = NULL;
    start = new Node(s, 0, NULL);
    start->h = dist(s,g);
    q.push(start);


    // keep searching the queue while it isn't empty, we haven't exceed max nodes and we haven't found the goal
    while(!q.empty() && node_count <= 1000 && !goal)    {
        current = q.top();
        q.pop();
        node_count++;

        if(verbose) cout << "Expanded " << current->value << " with h(n)=" << current->h << endl;

        // expand the kids, push them onto the back of the queue
        vector<Node*> kids = current->expand(forbid);
        for(unsigned int c = 0; c < kids.size(); c++) {
            tchild = kids[c];
            tchild->h = dist(tchild->value, g);
            q.push(tchild);
        }

        if(current->value == g) goal = current;
    }

    // calculate the time
    end_time = clock();
    total_time = end_time - start_time;

    cout << "\n\n### Results ###\n";

    if(goal) {
        cout << "The solution (backwards) is" << endl;
        int path_cost = 0;
        while(current) {
            cout << current->value;
            current = current->parent;
            if(current) cout << " -> ";
            path_cost++;
        }
        cout << endl << "Which has a path cost of " << path_cost;
    }
    else cout << "No solution was found" << endl;

    cout << endl;
    stats(node_count, node_count, total_time);
}

struct t_less_astar : binary_function <Node*, Node*, bool>
{
    bool operator()(Node * n1, Node * n2) {
        if ((n1->g + n1->h) >= (n2->g + n2->h)) return true;
        else return false;
    }
};

void a_star(int s, int g, const set<int> & forbid, bool verbose)
{
    Node * start, * goal, * current, * tchild;
    clock_t start_time, end_time, total_time;
    int node_count = 0;
    priority_queue<Node*, vector<Node*>, t_less_astar> q;

    cout << "\n### A* ###\n";
    start_time = clock();

    // Create the initial state, insert it on a queue
    goal = NULL;
    start = new Node(s, 0, NULL);
    start->h = dist(s,g);
    start->g = 0;
    q.push(start);

    // keep searching the queue while it isn't empty, we haven't exceed max nodes and we haven't found the goal
    while(!q.empty() && node_count <= 1000 && !goal) {
        current = q.top();
        q.pop();
        node_count++;

        if(verbose) cout<<"Expanded "<<current->value<<" with f(n)=h(n)+g(n)="<<current->h<<"+"<<current->g<<"="<<current->h+current->g<<endl;

        // expand the kids, push them onto the back of the queue
        vector<Node*> kids = current->expand(forbid);
        for(unsigned int c = 0; c < kids.size(); c++) {
            tchild = kids[c];
            tchild->h = dist(tchild->value, g);
            tchild->g = current->g + 1;
            q.push(tchild);
        }

        if(current->value == g) goal = current;
    }

    // calculate the time
    end_time = clock();
    total_time = end_time - start_time;

    cout << "\n\n### Results ###\n";

    if(goal) {
        cout << "The solution (backwards) is" << endl;
        int path_cost = 0;
        while(current) {
            cout << current->value;
            current = current->parent;
            if(current) cout << " -> ";
            path_cost++;
        }
        cout << endl << "Which has a path cost of " << path_cost;
    }
    else cout << "No solution was found" << endl;

    cout << endl;
    stats(node_count, node_count, total_time);
}


void dfs(int s, int g, const set<int> & forbid, bool verbose)
{
    Node * start, * goal, * current;
    clock_t start_time, end_time, total_time;
    int node_count = 0;
    stack<Node*> q;

    cout << "\n### Starting DFS ###\n";
    start_time = clock();

    // Create the initial state, insert it on a queue
    goal = NULL;
    start = new Node(s, 0, NULL);
    q.push(start);

    // keep searching the queue while it isn't empty, we haven't exceed max nodes and we haven't found the goal
    while(!q.empty() && node_count < 1000 && !goal) {
        current = q.top();
        q.pop();
        node_count++;

        if(verbose) cout << current->value << "; ";

        // expand the kids, push them onto the back of the queue
        vector<Node*> kids = current->expand(forbid);
        for(int c = kids.size() -1 ; c >=0 ; c--) {
            q.push(kids[c]);
        }

        if(current->value == g) goal = current;
    }

    // calculate the time
    end_time = clock();
    total_time = end_time - start_time;

    cout << "\n\n### Results ###\n";

    if(goal) {
        cout << "The solution (backwards) is" << endl;
        int path_cost = 0;
        while(current) {
            cout << current->value;
            current = current->parent;
            if(current) cout << " -> ";
            path_cost++;
        }
        cout << endl << "Which has a path cost of " << path_cost;
    }
    else cout << "No solution was found" << endl;

    cout << endl;
    stats(node_count, node_count, total_time);
}




void ids(int s, int g, const set<int> & forbid, bool verbose)
{
    Node * start, * goal, * current, * temp;
    clock_t start_time, end_time, total_time;
    int total_expanded, nodes_expanded = 0, depth_limit;
    stack<Node*> q;

    cout << "\n### Starting DFS ###\n";
    start_time = clock();

    // Create the initial state, insert it on a queue
    goal = NULL;

    depth_limit = 0;
    // keep increasing the depth of the search till we exceed 1000 expanded nodes or find a solution
    while(nodes_expanded < 1000 && !goal) {
        nodes_expanded = 0;
        start = new Node(s, 0, NULL);
        start->depth = 0;
        q.push(start);

        if(verbose) cout << "\n\nDepth " << depth_limit << endl;

        while(!q.empty() && nodes_expanded < 1000 && !goal) {
            current = q.top();
            if(current->value == g) goal = current;
            if(verbose) cout << current->value << " ";
            q.pop();


            // only expand if we can go deeper
            if(current->depth < depth_limit) {
                // expand the kids, push them onto the back of the queue
                vector<Node*> kids = current->expand(forbid);
                for(int c = kids.size() -1 ; c >=0 ; c--) {
                    nodes_expanded++;
                    temp = kids[c];
                    temp->depth = current->depth + 1;
                    q.push(temp);
                }
            }
            else if(!goal) {

            }

        }
        depth_limit++;
        total_expanded = total_expanded + nodes_expanded;
    }

    // calculate the time
    end_time = clock();
    total_time = end_time - start_time;

    cout << "Max depth was " << depth_limit << endl;
    cout << "\n\n### Results ###\n";

    if(goal) {
        cout << "The solution (backwards) is" << endl;
        int path_cost = 0;
        while(current) {
            cout << current->value;
            current = current->parent;
            if(current) cout << " -> ";
            path_cost++;
        }
        cout << endl << "Which has a path cost of " << path_cost;
    }
    else cout << "No solution was found" << endl;

    cout << endl;
    stats(0, total_expanded, total_time);
}

int main(int argc, char *argv[])
{
    set<int> forbidden;
    char tnum[3];
    int start, goal;
    bool verbose = false;

    // check for sufficient arguments
    if(argc < 3) {
        std::cout << "example usage: 3dp puzzle.txt d (verbose)" << endl;
        exit(-1);
    }

    // turn input puzzle file into stream and check the input file exists
    ifstream puzzlefile(argv[1]);
    if (!puzzlefile) {
        std::cout << "ERROR: the file " << argv[1] << " doesn't exist" << endl;
        exit(-1);
    }

    // parse the file
    puzzlefile.getline(tnum,4);
    start = atoi(tnum);
    puzzlefile.getline(tnum,4);
    goal = atoi(tnum);

    while (puzzlefile.good()) {
        puzzlefile.getline(tnum,4,',');
        forbidden.insert(atoi(tnum));
    }

    /* Close the file streams */
    puzzlefile.close();

    // if there is a forth argument turn on verbose
    if(argc == 4) verbose = true;


    // perform the selected search
    if(!strcmp(argv[2],"b")) bfs(start,goal,forbidden,verbose);
    else if (!strcmp(argv[2],"d")) dfs(start,goal,forbidden,verbose);
    else if (!strcmp(argv[2],"g")) greedy(start,goal,forbidden,verbose);
    else if (!strcmp(argv[2],"a")) a_star(start,goal,forbidden,verbose);
    else if (!strcmp(argv[2],"i")) ids(start,goal,forbidden,verbose);
}

