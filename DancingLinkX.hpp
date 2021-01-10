//
// Created by SiriusNEO on 2021/1/10.
//

#ifndef SUDOKUBREAKER_DANCINGLINKX_HPP
#define SUDOKUBREAKER_DANCINGLINKX_HPP

#include <iostream>
#include <vector>

class DancingLinkMatrix {
    public:
        std::vector<int> ans;
        DancingLinkMatrix(int _col, int _row, bool** mat) {
            colHeader = new Node [_col + 1];
            rowHeader = new Node [_row + 1];
            //self link
            for (int i = 0; i <= _col; ++i)
                colHeader[i].up = colHeader[i].down = &colHeader[i], colHeader[i].colIndex = i;
            for (int i = 1; i <= _row; ++i)
                rowHeader[i].left = rowHeader[i].right = &rowHeader[i];
            //col left-right link
            colHeader[0].left = &colHeader[_col], colHeader[_col].right = &colHeader[0];
            colHeader[0].right = &colHeader[1], colHeader[_col].left = &colHeader[_col-1];
            for (int i = 1; i < _col; ++i)
                colHeader[i].left = &colHeader[i-1], colHeader[i].right = &colHeader[i+1];
            for (int i = 1; i <= _row; ++i)
                for (int j = 1; j <= _col; ++j)
                    if (mat[i-1][j-1]) Insert(j, i);
        }
        ~DancingLinkMatrix() {
            Node *ptr = &colHeader[1];
            while (ptr != &colHeader[0]) {
                Node *ptr1 = ptr->down;
                while (ptr1 != ptr) {
                    Node *tmp = ptr1->down;
                    delete ptr1;
                    ptr1 = tmp;
                }
                ptr = ptr->right;
            }
            delete [] colHeader;
            delete [] rowHeader;
        }
        void Insert(int colIndex, int rowIndex) {
            Node *newNode = new Node(colIndex, rowIndex), *ptr = &colHeader[colIndex];
            //col insert
            colHeader[colIndex].size++;
            while (ptr->down->rowIndex < rowIndex && ptr->down != &colHeader[colIndex]) ptr = ptr->down; //find the insert pos
            newNode->down = ptr->down;
            newNode->up = ptr;
            ptr->down->up = newNode;
            ptr->down = newNode;
            //row insert
            ptr = &rowHeader[rowIndex];
            while (ptr->right->colIndex < colIndex && ptr->right != &rowHeader[rowIndex]) ptr = ptr->right;
            newNode->right = ptr->right;
            newNode->left = ptr;
            ptr->right->left = newNode;
            ptr->right = newNode;
        }
        void colDelete(int colIndex) {
            //delete col-header
            colHeader[colIndex].left->right = colHeader[colIndex].right;
            colHeader[colIndex].right->left = colHeader[colIndex].left;
            //delete col members
            Node *ptr = colHeader[colIndex].down;
            while (ptr != &colHeader[colIndex]) {
                //delete the whole row
                Node *ptr1 = ptr->right;
                while (ptr1 != ptr) {
                    if (ptr1 != &rowHeader[ptr->rowIndex]) {
                        ptr1->up->down = ptr1->down;
                        ptr1->down->up = ptr1->up;
                        colHeader[ptr1->colIndex].size--;
                    }
                    ptr1 = ptr1->right;
                }
                ptr = ptr->down;
            }
        }
        void colRollback(int colIndex) {
            colHeader[colIndex].right->left = colHeader[colIndex].left->right = &colHeader[colIndex];
            Node *ptr = colHeader[colIndex].down;
            while (ptr != &colHeader[colIndex]) {
                Node *ptr1 = ptr->right;
                while (ptr1 != ptr) {
                    if (ptr1 != &rowHeader[ptr->rowIndex]) {
                        ptr1->up->down = ptr1->down->up = ptr1;
                        colHeader[ptr1->colIndex].size++;
                    }
                    ptr1 = ptr1->right;
                }
                ptr = ptr->down;
            }
        }
        bool dfs() {
            if (colHeader[0].right == &colHeader[0]) return true; //empty
            Node *minCol = colHeader[0].right, *ptr = colHeader[0].right;
            while (ptr != &colHeader[0]) {
                if (ptr->size < minCol->size) minCol = ptr;
                if (minCol->size == 1) break;
                if (minCol->size <= 0) return false;
                ptr = ptr->right;
            }
            colDelete(minCol->colIndex);
            ptr = minCol->down;
            while (ptr != minCol) { //choose one row
                ans.push_back(ptr->rowIndex-1);
                Node *ptr1 = ptr->right;
                while (ptr1 != ptr) {
                    if (ptr1 != &rowHeader[ptr->rowIndex]) colDelete(ptr1->colIndex);
                    ptr1 = ptr1->right;
                }
                if (dfs()) return true;
                ptr1 = ptr->right;
                while (ptr1 != ptr) {
                    if (ptr1 != &rowHeader[ptr->rowIndex]) colRollback(ptr1->colIndex);
                    ptr1 = ptr1->right;
                }
                ans.pop_back();
                ptr = ptr->down;
            }
            colRollback(minCol->colIndex);
            return false;
        }

    private:
        struct Node {
            Node *left, *right, *up, *down;
            int colIndex, rowIndex, size;
            explicit Node(int _colIndex=0, int _rowIndex=0):
            left(nullptr), right(nullptr),up(nullptr),down(nullptr),colIndex(_colIndex),rowIndex(_rowIndex) {}
        };
        Node *colHeader, *rowHeader;
};

#endif //SUDOKUBREAKER_DANCINGLINKX_HPP
