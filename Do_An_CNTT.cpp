#include <iostream> 
#include <string.h>
#include <conio.h>
#include <queue>
#include <fcntl.h>
#include <corecrt_io.h>
#include <io.h>
#include <string>
#include <fstream>
#include <locale>
#include <codecvt>

using namespace std;
//khai báo thuộc tính color
enum COLOR { RED, BLACK };
//tạo cấy trúc node
class Node {
public:
    int key;
    wstring tu;
    wstring nghia;
    wstring loaitu;
    wstring vidu1;
    wstring gth1;
    wstring vidu2;
    wstring gth2;
    wstring vidu3;
    wstring gth3;
    COLOR color;
    Node* left, * right, * parent;

    Node(wstring tu, wstring nghia, wstring loaitu, wstring vidu1, wstring gth1, wstring vidu2, wstring gth2, wstring vidu3, wstring gth3)
        :tu(tu), nghia(nghia), loaitu(loaitu), vidu1(vidu1), gth1(gth1), vidu2(vidu2), gth2(gth2), vidu3(vidu3), gth3(gth3) {
        parent = left = right = NULL;
        // Nút được tạo trong quá trình chèn
        // Nút có màu đỏ khi chèn
        color = RED;
    }

    Node(wstring tu) :tu(tu) {
        parent = left = right = NULL;
        // Nút được tạo trong quá trình chèn
        // Nút có màu đỏ khi chèn
        color = RED;
    }

    Node(int key) :key(key) {
        parent = left = right = NULL;
        // Nút được tạo trong quá trình chèn
        // Nút có màu đỏ khi chèn
        color = RED;
    }

    // trả về con trỏ tới node chú
    Node* uncle() {
        // Nếu không có node cha hoặc node ông, thì không có node chú
        if (parent == NULL or parent->parent == NULL)
            return NULL;

        if (parent->isOnLeft())
            // node chú bên phải
            return parent->parent->right;
        else
            //node chú bên trái
            return parent->parent->left;
    }

    // kiểm tra xem node có phải là node con của cha không 
    bool isOnLeft() { return this == parent->left; }

    // trả về con trỏ cho node anh chị em
    Node* sibling() {
        // Node anh rỗng nếu không tồn tại node cha 
        if (parent == NULL)
            return NULL;

        if (isOnLeft())
            return parent->right;

        return parent->left;
    }

    // di chuyển nút xuống và di chuyển nút đã cho vào vị trí của nó
    void moveDown(Node* nParent) {
        if (parent != NULL) {
            if (isOnLeft()) {
                parent->left = nParent;
            }
            else {
                parent->right = nParent;
            }
        }
        nParent->parent = parent;
        parent = nParent;
    }

    bool hasRedChild() {
        return (left != NULL and left->color == RED) or
            (right != NULL and right->color == RED);
    }
};

class RBTree {
    Node* root;

    // xoay trái node đã cho 
    void leftRotate(Node* x) {
        // node cha mới sẽ là con bên phải của nút 
        Node* nParent = x->right;

        // cập nhật gốc nếu nút hiện tại là gốc 
        if (x == root)
            root = nParent;

        x->moveDown(nParent);

        // kết nối x với phần tử bên trái của cha mẹ mới 
        x->right = nParent->left;
        // kết nối phần tử bên trái của cha mẹ mới với nút 
        // nếu nó không phải là null 
        if (nParent->left != NULL)
            nParent->left->parent = x;

        // kết nối cha mẹ mới với x 
        nParent->left = x;
    }

    void rightRotate(Node* x) {
        // cha mẹ mới sẽ là con bên trái của nút
        Node* nParent = x->left;

        // cập nhật gốc nếu nút hiện tại là gốc
        if (x == root)
            root = nParent;

        x->moveDown(nParent);

        // kết nối x với phần tử bên phải của cha mẹ mới 
        x->left = nParent->right;
        //kết nối phần tử bên phải của cha mẹ mới với nút
        // nếu nó không phải là null
        if (nParent->right != NULL)
            nParent->right->parent = x;

        // kết nối cha mẹ mới với x
        nParent->right = x;
    }

    void swapColors(Node* x1, Node* x2) {
        COLOR temp;
        temp = x1->color;
        x1->color = x2->color;
        x2->color = temp;
    }

    void swapkeyues(Node* u, Node* v) {
        wstring tu1 = u->tu;
        wstring loaitu1 = u->loaitu;
        wstring nghia1 = u->nghia;
        wstring vd1 = u->vidu1;
        wstring g1 = u->gth1;
        wstring vd2 = u->vidu2;
        wstring g2 = u->gth2;
        wstring vd3 = u->vidu3;
        wstring g3 = u->gth3;

        u->tu = v->tu;
        u->nghia = v->nghia;
        u->loaitu = v->loaitu;
        u->vidu1 = v->vidu1;
        u->gth1 = v->gth1;
        u->vidu2 = v->vidu2;
        u->gth2 = v->gth2;
        u->vidu3 = v->vidu3;
        u->gth3 = v->gth3;

        v->tu = tu1;
        v->nghia = nghia1;
        v->loaitu = loaitu1;
        v->vidu1 = vd1;
        v->gth1 = g1;
        v->vidu2 = vd2;
        v->gth2 = g2;
        v->vidu3 = vd3;
        v->gth3 = g3;
    }

    // sửa màu đỏ đỏ tại nút nhất định
    void fixRedRed(Node* x) {
        // nếu x là màu gốc, nó là màu đen và trả về 
        if (x == root) {
            x->color = BLACK;
            return;
        }

        // khởi tạo cha mẹ, ông bà, chú
        Node* parent = x->parent, * grandparent = parent->parent,
            * uncle = x->uncle();

        if (parent->color != BLACK) {
            if (uncle != NULL && uncle->color == RED) {
                // chú màu đỏ, thực hiện tô màu và đệ quy
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                fixRedRed(grandparent);
            }
            else {
                // Các hoạt động khác LR, LL, RL, RR
                if (parent->isOnLeft()) {
                    if (x->isOnLeft()) {
                        // cho left right 
                        swapColors(parent, grandparent);
                    }
                    else {
                        leftRotate(parent);
                        swapColors(x, grandparent);
                    }
                    // cho left left và left right 
                    rightRotate(grandparent);
                }
                else {
                    if (x->isOnLeft()) {
                        // cho right left 
                        rightRotate(parent);
                        swapColors(x, grandparent);
                    }
                    else {
                        swapColors(parent, grandparent);
                    }

                    // cho right right và right left 
                    leftRotate(grandparent);
                }
            }
        }
    }

    // tìm nút không có nút con bên trái 
    // trong cây con của nút đã cho 
    Node* successor(Node* x) {
        Node* temp = x;

        while (temp->left != NULL)
            temp = temp->left;

        return temp;
    }

    // tìm nút thay thế nút đã xóa trong BST 
    Node* BSTreplace(Node* x) {
        // khi nút có 2 con
        if (x->left != NULL and x->right != NULL)
            return successor(x->right);

        // khi node lá 
        if (x->left == NULL and x->right == NULL)
            return NULL;

        // khi node có một con
        if (x->left != NULL)
            return x->left;
        else
            return x->right;
    }

    // xóa nút đã cho
    void deleteNode(Node* v) {
        Node* u = BSTreplace(v);

        // Đúng khi u và v đều đen 
        bool uvBlack = ((u == NULL or u->color == BLACK) and (v->color == BLACK));
        Node* parent = v->parent;

        if (u == NULL) {
            // u là NULL do đó v là lá 
            if (v == root) {
                // v là root, làm cho root là null 
                root = NULL;
            }
            else {
                if (uvBlack) {
                    // u và v đều đen
                    // v là lá, sửa màu đen kép tại v 
                    fixDoubleBlack(v);
                }
                else {
                    // u hoặc v là đỏ
                    if (v->sibling() != NULL)
                        // node anh chị em không rỗng, làm cho nó màu đỏ" 
                        v->sibling()->color = RED;
                }

                // xóa v khỏi cây 
                if (v->isOnLeft()) {
                    parent->left = NULL;
                }
                else {
                    parent->right = NULL;
                }
            }
            delete v;
            return;
        }

        if (v->left == NULL or v->right == NULL) {
            // v có 1 node con
            if (v == root) {
                // v là gốc, gán giá trị của u cho v và xóa u 
                v->tu = u->tu;
                v->left = v->right = NULL;
                delete u;
            }
            else {
                // Tách v khỏi cây và di chuyển u lên
                if (v->isOnLeft()) {
                    parent->left = u;
                }
                else {
                    parent->right = u;
                }
                delete v;
                u->parent = parent;
                if (uvBlack) {
                    // u và v đều đen, sửa hai màu đen ở u 
                    fixDoubleBlack(u);
                }
                else {
                    // u hoặc v đỏ, màu u đen 
                    u->color = BLACK;
                }
            }
            return;
        }

        // v có 2 con, hoán đổi giá trị với kế nhiệm và đệ quy 
        swapkeyues(u, v);
        deleteNode(u);
    }

    void fixDoubleBlack(Node* x) {
        if (x == root)
            // x là node gốc thì return 
            return;

        Node* sibling = x->sibling(), * parent = x->parent;
        if (sibling == NULL) {
            // Không có sibiling, màu đen kép được đẩy lên 
            fixDoubleBlack(parent);
        }
        else {
            if (sibling->color == RED) {
                // Anh chị em màu đỏ 
                parent->color = RED;
                sibling->color = BLACK;
                if (sibling->isOnLeft()) {
                    // trường hợp left 
                    rightRotate(parent);
                }
                else {
                    // trường hợp right 
                    leftRotate(parent);
                }
                fixDoubleBlack(x);
            }
            else {
                // Anh chị em đen 
                if (sibling->hasRedChild()) {
                    // ít nhất 1 trẻ em màu đỏ 
                    if (sibling->left != NULL and sibling->left->color == RED) {
                        if (sibling->isOnLeft()) {
                            // left left 
                            sibling->left->color = sibling->color;
                            sibling->color = parent->color;
                            rightRotate(parent);
                        }
                        else {
                            // right left 
                            sibling->left->color = parent->color;
                            rightRotate(sibling);
                            leftRotate(parent);
                        }
                    }
                    else {
                        if (sibling->isOnLeft()) {
                            // left right 
                            sibling->right->color = parent->color;
                            leftRotate(sibling);
                            rightRotate(parent);
                        }
                        else {
                            // right right 
                            sibling->right->color = sibling->color;
                            sibling->color = parent->color;
                            leftRotate(parent);
                        }
                    }
                    parent->color = BLACK;
                }
                else {
                    // hai con đen 
                    sibling->color = RED;
                    if (parent->color == BLACK)
                        fixDoubleBlack(parent);
                    else
                        parent->color = BLACK;
                }
            }
        }
    }


    // in đệ quy order
    void inorder(Node* x) {
        if (x == NULL)
            return;
        inorder(x->left);
        wcout << x->tu << L"\n";
        inorder(x->right);
    }

public:
    RBTree() { root = NULL; }

    Node* getRoot() { return root; }
    Node* search(wstring n) {
        Node* temp = root;
        while (temp != NULL) {
            if (n < temp->tu) {
                if (temp->left == NULL)
                    break;
                else
                    temp = temp->left;
            }
            else if (n == temp->tu) {
                break;
            }
            else {
                if (temp->right == NULL)
                    break;
                else
                    temp = temp->right;
            }
        }

        return temp;
    }

    Node* search(int n) {
        Node* temp = root;
        while (temp != NULL) {
            if (n < temp->key) {
                if (temp->left == NULL)
                    break;
                else
                    temp = temp->left;
            }
            else if (n == temp->key) {
                break;
            }
            else {
                if (temp->right == NULL)
                    break;
                else
                    temp = temp->right;
            }
        }

        return temp;
    }

    // chen giá trị đã cho vào cây
    void insert(wstring tu, wstring nghia, wstring loaitu, wstring vidu1, wstring gth1, wstring vidu2, wstring gth2, wstring vidu3, wstring gth3) {
        Node* newNode = new Node(tu, nghia, loaitu, vidu1, gth1, vidu2, gth2, vidu3, gth3);
        if (root == NULL) {
            newNode->color = BLACK;
            root = newNode;
        }
        else {
            Node* temp = search(tu);

            if (temp->tu == tu) {
                return;
            }
            newNode->parent = temp;

            if (tu < temp->tu)
                temp->left = newNode;
            else
                temp->right = newNode;

            fixRedRed(newNode);
        }
    }

    void insert(wstring tu) {
        Node* newNode = new Node(tu);
        if (root == NULL) {
            newNode->color = BLACK;
            root = newNode;
        }
        else {
            Node* temp = search(tu);

            if (temp->tu == tu) {
                return;
            }
            newNode->parent = temp;

            if (tu < temp->tu)
                temp->left = newNode;
            else
                temp->right = newNode;

            fixRedRed(newNode);
        }
    }

    void insert(int n) {
        Node* newNode = new Node(n);
        if (root == NULL) {
            newNode->color = BLACK;
            root = newNode;
        }
        else {
            Node* temp = search(n);

            if (temp->key == n) {
                return;
            }
            newNode->parent = temp;

            if (n < temp->key)
                temp->left = newNode;
            else
                temp->right = newNode;

            fixRedRed(newNode);
        }
    }

    // chức năng tiện ích xóa nút có giá trị nhất định
    void deleteBykey(wstring n) {
        if (root == NULL)
            // Tree is empty 
            return;

        Node* v = search(n);

        if (v->tu != n) {
            wcout << L"Không tìm thấy từ " << n << L"trong từ điển để xóa!\n";
        }
        else {
            deleteNode(v);
            wcout << L"Xóa thành công!\n";
        }
    }

    // in theo thứ tự 
    void printInOrder() {
        wcout << L"In theo thứ tự: \n";
        if (root == NULL)
            wcout << L"Cây rỗng" << endl;
        else
            inorder(root);
    }

    // chuyển tất cả chữ cái trong 1 từ về dạng chữ thường
    wstring lower(wstring tu) {
        for (int i = 0; i < tu.size(); i++) {
            if (!islower(tu[i]))
                tu[i] += 32;
        }
        return tu;
    }
};

int main() {
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);

    RBTree tree;

    wstring tu1;
    wstring nghia1;
    wstring loaitu1;
    wstring vd1;
    wstring g1;
    wstring vd2;
    wstring g2;
    wstring vd3;
    wstring g3;
    wstring r;
    locale loc(locale(), new codecvt_utf8<wchar_t>);

    //đọc file
    wifstream fin;
    fin.open(L"D:\\HK5_2021-2022\\DoAnCNTT\\tudien.txt");
    if (!fin) {
        wcout << L"Không thể đọc file.";
    }
    else {
        fin.imbue(loc);
        getline(fin, r);
        while (!fin.eof())
        {
            getline(fin, tu1);
            getline(fin, nghia1);
            getline(fin, loaitu1);
            getline(fin, vd1);
            getline(fin, g1);
            getline(fin, vd2);
            getline(fin, g2);
            getline(fin, vd3);
            getline(fin, g3);
            tree.insert(tu1, nghia1, loaitu1, vd1, g1, vd2, g2, vd3, g3);
        }
    }
    fin.close();

    do {
        int choose;
        wcout << L"\t------Từ điển Anh - Việt------\n";
        wcout << L"\t-------------------------------\n";
        wcout << L"\tChọn 1 để thêm vào từ điển.\n";
        wcout << L"\tChọn 2 để xóa 1 từ ra khỏi từ điển.\n";
        wcout << L"\tChọn 3 để tra cứu từ điển.\n";
        wcout << L"\tChọn 4 để xem tất cả từ trong từ điển.\n";
        wcout << L"\tChọn 0 để thoát chương trình.\n";
        wcout << L"\t-------------------------------\n";
        wcout << L"\tLƯU Ý: Nếu nhập ký tự hoặc chuỗi thì chương trình sẽ tự động kết thúc!\n";

        do {
            wcout << L"Lựa chọn của bạn là: ";
            wcin >> choose;
            if (choose < 0 || choose > 4) wcout << L"Vui lòng chọn đúng chức năng (trong phạm vi từ 0 đến 3)!\n";
        } while (choose < 0 || choose > 4);

        wcin.ignore();

        if (choose == 1) {
            wcout << L"---Thêm vào từ điển---\n";
            wcout << L"Nhập từ muốn thêm: "; getline(wcin, tu1);
            Node* y = tree.search(tu1);
            if (y->tu == tu1)
                wcout << L"Từ này đã có trong từ điển!\n";
            else {
                wcout << L"Nhập nghĩa của từ: "; getline(wcin, nghia1);
                wcout << L"Nhập từ loại: "; getline(wcin, loaitu1);
                wcout << L"Nhập ví dụ 1: "; getline(wcin, vd1);
                wcout << L"Nhập nghĩa của ví dụ 1: "; getline(wcin, g1);
                wcout << L"Nhập ví dụ 2: "; getline(wcin, vd2);
                wcout << L"Nhập nghĩa của ví dụ 2: "; getline(wcin, g2);
                wcout << L"Nhập ví dụ 3: "; getline(wcin, vd3);
                wcout << L"Nhập nghĩa của ví dụ 3: "; getline(wcin, g3);
                tu1 = tree.lower(tu1);
                tree.insert(tu1, nghia1, loaitu1, vd1, g1, vd2, g2, vd3, g3);
                //ghi file
                wofstream fout;
                fout.open(L"D:\\HK5_2021-2022\\DoAnCNTT\\tudien.txt", ios::app);
                fout.imbue(loc);
                fout << L"\n" << tu1 << L"\n" << nghia1 << L"\n" << loaitu1 << L"\n" << vd1 << L"\n" << g1 << L"\n"
                    << vd2 << L"\n" << g2 << L"\n" << vd3 << L"\n" << g3;
                wcout << L"Thêm thành công!\n";
                fout.close();
            }
        }
        else if (choose == 2)
        {
            wcout << L"---Xóa khỏi từ điển---\n";
            wcout << L"Nhập từ muốn xóa: "; getline(wcin, tu1);
            tu1 = tree.lower(tu1);
            tree.deleteBykey(tu1);
            //------------------------
            wfstream goc;
            wfstream tam;
            goc.open(L"D:\\HK5_2021-2022\\DoAnCNTT\\tudien.txt", ios::in);
            tam.open(L"tam.txt", ios::out);
            wstring tu2;
            wstring nghia2;
            wstring lt2;
            wstring vda;
            wstring ga;
            wstring vdb;
            wstring gb;
            wstring vdc;
            wstring gc;
            goc.imbue(loc);
            tam.imbue(loc);
            getline(goc, r);
            while (!goc.eof())
            {
                getline(goc, tu2);
                getline(goc, nghia2);
                getline(goc, lt2);
                getline(goc, vda);
                getline(goc, ga);
                getline(goc, vdb);
                getline(goc, gb);
                getline(goc, vdc);
                getline(goc, gc);
                if (tu1 == tu2)
                {
                    continue;
                }
                else
                {
                    tam << L"\n" << tu2 << L"\n" << nghia2 << L"\n" << lt2 << L"\n" << vda << L"\n" << ga << L"\n"
                        << vdb << L"\n" << gb << L"\n" << vdc << L"\n" << gc;
                }
            }
            goc.close();
            tam.close();

            goc.open(L"D:\\HK5_2021-2022\\DoAnCNTT\\tudien.txt", ios::out);
            tam.open(L"tam.txt", ios::in);
            goc.imbue(loc);
            tam.imbue(loc);

            getline(tam, r);
            while (!tam.eof())
            {
                getline(tam, tu2);
                getline(tam, nghia2);
                getline(tam, lt2);
                getline(tam, vda);
                getline(tam, ga);
                getline(tam, vdb);
                getline(tam, gb);
                getline(tam, vdc);
                getline(tam, gc);
                goc << L"\n" << tu2 << L"\n" << nghia2 << L"\n" << lt2 << L"\n" << vda << L"\n" << ga << L"\n"
                    << vdb << L"\n" << gb << L"\n" << vdc << L"\n" << gc;
            }
            tam.close();
            goc.close();
            remove("tam.txt");
            //------------------------

        }
        else if (choose == 3)
        {
            wcout << L"----Tìm kiếm----\n";
            //wcin.ignore();
            wcout << L"Nhập từ muốn tìm: "; getline(wcin, tu1);
            tu1 = tree.lower(tu1);
            Node* x = tree.search(tu1);
            if (x->tu == tu1)
            {
                wcout << L"-----Kết quả-----\n";
                wcout << x->tu << endl;
                wcout << L"Nghĩa: " << x->nghia << endl;
                wcout << L"Từ loại: " << x->loaitu << endl;
                wcout << L"Ví dụ 1: " << x->vidu1 << endl;
                wcout << L"=> " << x->gth1 << endl;
                wcout << L"Ví dụ 2: " << x->vidu2 << endl;
                wcout << L"=> " << x->gth2 << endl;
                wcout << L"Ví dụ 3: " << x->vidu3 << endl;
                wcout << L"=> " << x->gth3 << endl;
                wcout << L"-------------------\n";
            }
            else wcout << L"Xin lỗi bạn! Từ điển chưa cập nhật từ này.\n";
        }
        else if (choose == 4) {
            wcout << L"---Danh sách từ điển---\n";
            tree.printInOrder();
        }
        else if (choose == 0)
        {
            wcout << L"--Cảm ơn bạn đã sử dụng Từ điển--";
            break;
        }
        wcout << L"Nhấn phím bất kỳ để tiếp tục hoặc Nhấn phím ESC để thoát chương trình.\n\n\n";

    } while (_getch() != 27);
}