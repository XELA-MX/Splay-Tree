#ifndef SPLAY_H
#define SPLAY_H

#include <string>
#include <sstream>


template <class T>
class SplayTree {
private:
    // Estructura de un nodo
    struct Node {
        T value;
        Node* left;
        Node* right;
        Node* parent;

        Node(T val) : value(val), left(nullptr), right(nullptr), parent(nullptr) {}
    };

    Node* root;

    // Zig derecha
    void rotateRight(Node* x) {
        Node* y = x->left;
        x->left = y->right;
        if (y->right) y->right->parent = x;
        y->parent = x->parent;
        if (!x->parent) root = y;
        else if (x == x->parent->right) x->parent->right = y;
        else x->parent->left = y;
        y->right = x;
        x->parent = y;
    }

    // Zig izquierda
    void rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left) y->left->parent = x;
        y->parent = x->parent;
        if (!x->parent) root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    // Movimientos zig y zig-zag para llevar un nodo a la raíz
    void splay(Node* x) {
        while (x->parent) {
            if (!x->parent->parent) {
                if (x == x->parent->left) rotateRight(x->parent);
                else rotateLeft(x->parent);
            } else if (x == x->parent->left && x->parent == x->parent->parent->left) {
                rotateRight(x->parent->parent);
                rotateRight(x->parent);
            } else if (x == x->parent->right && x->parent == x->parent->parent->right) {
                rotateLeft(x->parent->parent);
                rotateLeft(x->parent);
            } else if (x == x->parent->right && x->parent == x->parent->parent->left) {
                rotateLeft(x->parent);
                rotateRight(x->parent);
            } else {
                rotateRight(x->parent);
                rotateLeft(x->parent);
            }
        }
    }

    // Encontrar y llevar a la raíz
    Node* findNode(T val) {
        Node* current = root;
        while (current) {
            if (val == current->value) {
                splay(current);
                return current;
            }
            if (val < current->value) current = current->left;
            else current = current->right;
        }
        return nullptr;
    }

    // inorden
    void inorderTraversal(Node* node, std::stringstream& ss) {
        if (node) {
            inorderTraversal(node->left, ss);
            ss << node->value << " ";
            inorderTraversal(node->right, ss);
        }
    }

    // Preorden
    void preorderTraversal(Node* node, std::stringstream& ss) {
        if (node) {
            ss << node->value << " ";
            preorderTraversal(node->left, ss);
            preorderTraversal(node->right, ss);
        }
    }

public:
    SplayTree() : root(nullptr) {}

    // Nuevo valor
    void add(T val) {
        if (!root) {
            root = new Node(val);
            return;
        }
        Node* current = root;
        Node* parent = nullptr;
        while (current) {
            parent = current;
            if (val < current->value) current = current->left;
            else if (val > current->value) current = current->right;
            else return;  // El valor ya existe
        }
        Node* newNode = new Node(val);
        newNode->parent = parent;
        if (val < parent->value) parent->left = newNode;
        else parent->right = newNode;
        splay(newNode);
    }

    // Buscar en árbol
    bool find(T val) {
        Node* node = findNode(val);
        return node != nullptr;
    }

    // Ahora si eliminar
    void remove(T val) {
        Node* node = findNode(val);
        if (!node) return;

        Node* left = node->left;
        Node* right = node->right;

        if (!left) {
            root = right;
            if (right) right->parent = nullptr;
        } else if (!right) {
            root = left;
            left->parent = nullptr;
        } else {
            left->parent = nullptr;
            root = left;
            Node* rightmost = left;
            while (rightmost->right) rightmost = rightmost->right;
            splay(rightmost);
            rightmost->right = right;
            right->parent = rightmost;
        }

        delete node;
    }

    // A string en inorden
    std::string inorder() {
        std::stringstream ss;
        ss << "[";
        inorderTraversal(root, ss);
        std::string result = ss.str();
        if (result.length() > 1) result.pop_back();  // Elimina el último espacio
        result += "]";
        return result;
    }

    // A string preorden
    std::string preorder() {
        std::stringstream ss;
        ss << "[";
        preorderTraversal(root, ss);
        std::string result = ss.str();
        if (result.length() > 1) result.pop_back();  // Elimina el último espacio
        result += "]";
        return result;
    }
};

#endif // SPLAY_H