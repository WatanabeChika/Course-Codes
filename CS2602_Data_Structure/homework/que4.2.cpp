# include <iostream>

using namespace std;


struct treenode {
    char data;
    int sign;
    treenode *left, *right;

    treenode(const char& d, const int& s, treenode *l = NULL, treenode *r = NULL): data(d), sign(s), left(l), right(r) {};
};

class Btree {
    private:
        treenode *root;
        char array[2000] = "";
        int maxsign = 0;

        treenode *pre_in_to_order(char *preo, char *ino, const int& pl, const int& pr, const int& ml, const int& mr, const int& sign) {
            if (pl>pr) return NULL;

            treenode *target, *leftroot, *rightroot;
            int i, num;
            int lpl, lpr, rpl, rpr, lml, lmr, rml, rmr;

            target = new treenode(preo[pl], sign);
            if (!root) root = target;

            for (i = ml; i <= mr; ++i) {
                if (ino[i] == preo[pl]) break;
            }
            num = i-ml;

            lml = ml; lmr = i-1;
            rml = i+1; rmr = mr;
            lpl = pl+1; lpr = pl+num;
            rpl = lpr+1; rpr = pr;

            leftroot = pre_in_to_order(preo, ino, lpl, lpr, lml, lmr, 2*sign);
            rightroot = pre_in_to_order(preo, ino, rpl, rpr, rml, rmr, 2*sign+1);
            target->left = leftroot;
            target->right = rightroot;

            array[sign] = target->data;
            if (isalnum(array[sign]) && sign > maxsign) maxsign = sign;
            return target;
        }
    
        void postorder(treenode *p) {
            if(!p) return;
            postorder(p->left);
            postorder(p->right);
            delete p;
        }

    public:
        Btree(char *preo, char *ino, const int& pl, const int& pr, const int& ml, const int& mr) {
            root = pre_in_to_order(preo, ino, pl, pr, ml, mr, 1);
        }

        void level_order() const {
            for (int i = 1; i <= maxsign; ++i) {
                if (array[i] == '\0') cout << "NULL";
                else cout << array[i];
                cout << ' ';
            }
        }

        ~Btree() {
            postorder(root);
        }
        
};


int main() 
{
    char preOrder[30]="", inOrder[30]="";
    int i = 0;

    cin >> preOrder >> inOrder;
    while (preOrder[i] != '\0') ++i;

    Btree tree(preOrder, inOrder, 0, i, 0, i);

    tree.level_order();
    cout << endl;

    return 0;
}