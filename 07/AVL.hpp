template <class Key, class T, class Compare = std::less<Key>,
          class Allocator = std::allocator<std::pair<const Key, T>>>
class AVL {
    // узел дерева
    struct Node {
       public:
        Key key_;
        T value_;
        Node* parent;
        Node* left;
        Node* right;

        Node() {
            left = nullptr;
            right = nullptr;
            parent = nullptr;
        }

        Node(Key k, T v) {
            key_ = k;
            value_ = v;

            left = nullptr;
            right = nullptr;
            parent = nullptr;
        }
    };

    // итератор
    class Iterator {
        friend class AVL;
        using reference = T&;

       private:
        Node* ptr_;

       public:
        Iterator() { this->ptr_ = nullptr; }

        Iterator(Node* ptr) { this->ptr_ = ptr; }

        // конструктор копирования
        Iterator(const Iterator& it) { ptr_ = it.ptr_; }

        // оператор копирования
        Iterator& operator=(const Iterator& it) {
            ptr_ = it.ptr_;
            return *this;
        }

        bool operator==(const Iterator& it) const {
            return this->ptr_ == it.ptr_;
        }
        bool operator!=(const Iterator& it) const {
            return this->ptr_ != it.ptr_;
        }
        bool operator>(const Iterator& it) const { return **this > *it; }
        bool operator<(const Iterator& it) const { return **this < *it; }
        bool operator>=(const Iterator& it) const { return **this >= *it; }
        bool operator<=(const Iterator& it) const { return **this <= *it; }

        // ++iter
        Iterator& operator++() {
            // должны уйти в минимум из большего поддерева
            // это как раз будет следующее значение после текущего
            if (ptr_->right) {
                ptr_ = ptr_->right;
                while (ptr_->left) {
                    ptr_ = ptr_->left;
                }
            } else {
                // а если правого поддерева нет, то мы должны оказаться
                // в предке, для которого текущее значение находится
                // в левом поддереве
                Node* prev_node;
                do {
                    prev_node = ptr_;
                    ptr_ = ptr_->parent;
                } while (ptr_ && prev_node == ptr_->right);
            }
            return *this;
        }

        // iter++
        Iterator operator++(int) {
            Iterator old(*this);
            ++(*this);
            return old;
        }

        // --iter
        Iterator& operator--() {
            // должны уйти в максимум из меньшего поддерева
            // это как раз будет значение перед текущим
            if (ptr_->left) {
                ptr_ = ptr_->left;
                while (ptr_->right) {
                    ptr_ = ptr_->right;
                }
            } else {
                // а если левого поддерева нет, то мы должны оказаться
                // в предке, для которого текущее значение находится
                // в правом поддереве
                Node* prev_node;
                do {
                    prev_node = ptr_;
                    ptr_ = ptr_->parent;
                } while (ptr_ && prev_node == ptr_->left);
            }
            return *this;
        }

        // iter--
        Iterator operator--(int) {
            Iterator old(*this);
            --(*this);
            return old;
        }

        // оператор *ptr
        reference operator*() const { return ptr_->value_; }
    };

   public:
    Node* root_;
    size_t size_;
    using reference = T&;

    AVL() {
        root_ = nullptr;
        size_ = 0;
    }

    // проверка на пустоту дерева
    bool empty() { return root_ == nullptr; }

    // получить размер дерева
    size_t size() { return size_; }

    // очистка дерева
    void clear() { root_ = nullptr; }

    // минимальное значение в дереве для начала итератора
    Iterator begin() { return Iterator(minNode(root_)); }
    // для реверсивного итератора
    Iterator rbegin() { return Iterator(maxNode(root_)); }

    // максимальное значение в дереве для окончания итератора
    Iterator end() { return Iterator(maxNode(root_)); }
    // для реверсивного итератора
    Iterator rend() { return Iterator(minNode(root_)); }

    reference operator[](int i) { return *at(i); }

    Iterator at(size_t idx) {
        if (idx >= size()) {
            throw std::invalid_argument("The element does not exist!");
        }

        auto cur_val = begin();
        auto end_val = end();
        size_t cnt = 0;
        while (cur_val != end_val) {
            if (cnt == idx) {
                return cur_val;
            }
            ++cur_val;
            cnt++;
        }
        return nullptr;
    }

    int8_t height(Node* ptr) {
        if (ptr == nullptr) {
            return -1;
        } else {
            int8_t height_l = height(ptr->left);
            int8_t height_r = height(ptr->right);

            // высота выбирается по большему
            // из поддеревьев
            // 1 добавляем для учета текущей вершины
            if (height_l > height_r) {
                return (height_l + 1);
            } else {
                return (height_r + 1);
            }
        }
    }

    // поиск ноды по значению
    Node* find(Node* root, Key val) {
        // либо искомое значение в корне,
        // либо идем в одно из поддеревьев
        if (root == nullptr || root->key_ == val) {
            return root;
        } else if (val < root->key_) {
            return find(root->left, val);
        } else {
            return find(root->right, val);
        }
    }

    // проверка на наличие значения в дереве
    bool contains(Key key_) {
        Node* node = find(this->root_, key_);
        return node == nullptr ? false : true;
    }

    // обновление дерева после вставки/удаления
    void update(Node* root) {
        if (root == nullptr) {
            return;
        }
        if (root == this->root_) {
            root->parent = nullptr;
        }

        Node* left_node = root->left;
        Node* right_node = root->right;

        if (left_node != nullptr) {
            left_node->parent = root;
        }
        if (right_node != nullptr) {
            right_node->parent = root;
        }

        update(root->left);
        update(root->right);
    }

    // проверка на сбалансированность узла
    int8_t getBalanceFactor(Node* ptr) {
        if (ptr == nullptr) {
            return -1;
        }
        // сбалансированность дерева зависит от разницы
        // в высоте левого и правого поддеревьев
        return height(ptr->left) - height(ptr->right);
    }

    // правый поворот вокруг p
    Node* rightRotate(Node* p) {
        Node* q = p->left;
        Node* tmp_node = q->right;

        q->right = p;
        p->left = tmp_node;
        return q;
    }

    // левый поворот вокруг q
    Node* leftRotate(Node* q) {
        Node* p = q->right;
        Node* tmp_node = p->left;

        p->left = q;
        q->right = tmp_node;
        return p;
    }

    Node* deleteNode(Node* root, Key v) {
        if (root == nullptr) {
            return nullptr;
        }
        // если значение в левом поддереве
        else if (std::less<Key>{}(v, root->key_)) {
            root->left = deleteNode(root->left, v);
        }
        // если значение в право поддереве
        else if (std::less<Key>{}(root->key_, v)) {
            root->right = deleteNode(root->right, v);
        }
        // требуется удаление корня
        else {
            // нода с одним поддеревом (правым)
            if (root->left == nullptr) {
                Node* new_root = root->right;
                delete root;
                return new_root;
            }
            // нода с одним поддеревом (левым)
            else if (root->right == nullptr) {
                Node* new_root = root->left;
                delete root;
                return new_root;
            } else {
                // нужно найти наименьшее значение, превышающее значение в корне
                // это будет наименьшее значение в правом поддереве
                Node* new_root = minNode(root->right);
                // подставляем его вместо корня
                root->key_ = new_root->key_;
                // и удаляем на старом месте
                root->right = deleteNode(root->right, new_root->key_);
            }
        }

        // балансировка
        int8_t bf = getBalanceFactor(root);

        if (bf == 2 && getBalanceFactor(root->left) >= 0)
            return rightRotate(root);

        else if (bf == 2 && getBalanceFactor(root->left) == -1) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }

        else if (bf == -2 && getBalanceFactor(root->right) <= 0)
            return leftRotate(root);

        else if (bf == -2 && getBalanceFactor(root->right) == 1) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }

    // удаление значения
    Node* erase(Key key_) {
        Node* erase_node = find(root_, key_);
        if (erase_node != nullptr) {
            root_ = deleteNode(root_, key_);
        } else {
            throw std::invalid_argument("The value does not exist!");
        }
        size_--;
        update(root_);
        return root_;
    }

    // вставка в дерево
    Node* insert(Key k, T v) {
        Node* new_node = new Node(k, v);
        root_ = insertNode(root_, new_node);
        size_++;
        update(root_);
        return root_;
    }

    Node* insertNode(Node* root, Node* new_node) {
        if (root == nullptr) {
            root = new_node;
            return root;
        }

        // смотрим куда вставлять – левое или правое поддерево
        // делаем это рекурсивно
        if (std::less<Key>{}(new_node->key_, root->key_)) {
            root->left = insertNode(root->left, new_node);
        } else if (std::less<Key>{}(root->key_, new_node->key_)) {
            root->right = insertNode(root->right, new_node);
        } else {
            throw std::invalid_argument(
                "The value is already contained in the tree!");
            return root;
        }

        // в зависимости от фактора балансировки
        // будем делать левое/правое малое/большое вращение
        // https://en.wikipedia.org/wiki/AVL_tree#Rebalancing
        int8_t bf = getBalanceFactor(root);

        // Left Left
        if ((bf > 0) && std::less<Key>{}(new_node->key_, root->left->key_))
            return rightRotate(root);

        // Right Right
        if ((bf < 0) && std::less<Key>{}(root->right->key_, new_node->key_))
            return leftRotate(root);

        // Left Right
        if ((bf > 0) && std::less<Key>{}(root->left->key_, new_node->key_)) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }

        // Right Left
        if ((bf < 0) && std::less<Key>{}(root->right->key_, new_node->key_)) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }

    Node* minNode(Node* ptr) {
        Node* curr = new Node();
        curr = ptr;
        // минимальное значение – самое левое
        while (curr->left != nullptr) {
            curr = curr->left;
        }
        return curr;
    }

    Node* maxNode(Node* ptr) {
        Node* curr = new Node();
        curr = ptr;
        // максимальное значение – самое правое
        while (curr->right != nullptr) {
            curr = curr->right;
        }
        return curr;
    }
};