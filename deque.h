template <typename T>
class Deque {
private:
  T** outer;
  T* first_arr;
  T* last_arr;
  int first_index;
  int last_index;
  int first_arr_num;
  int last_arr_num;
  int arrays;
  int number_of_elements;
  static const int array_size = 32;

public:
  Deque() {
    outer = new T*[2];
    T* newarr1;
    try {
      newarr1 = reinterpret_cast<T *>(new char[array_size * sizeof(T)]);
    } catch (...) {
      delete[] outer;
    }
    T* newarr2;
    try {
      newarr2 = reinterpret_cast<T *>(new char[array_size * sizeof(T)]);
    } catch (...) {
      delete[] outer;
      delete newarr1;
      throw;
    }

    outer[0] = newarr1;
    outer[1] = newarr2;
    first_arr = outer[0];
    last_arr = outer[1];
    first_arr_num = 0;
    last_arr_num = 1;
    first_index = array_size - 1;
    last_index = 0;
    number_of_elements = 0;
    arrays = 2;
  };
  Deque(const Deque& other) : Deque() {
    const_iterator iter = other.cbegin();
    int j = 0;
    try {
      while (j < other.number_of_elements) {
        push_back(*(iter + j));
        ++j;
      }
    } catch (...) {
      while (j > 0) {
        delete (this + j);
        --j;
      }
      throw;
    }
  }
  Deque operator=(const Deque& other) {
    Deque temp = other;
    std::swap(outer, temp.outer);
    std::swap(first_arr, temp.first_arr);
    std::swap(last_arr, temp.last_arr);
    std::swap(first_arr_num, temp.first_arr_num);
    std::swap(last_arr_num, temp.last_arr_num);
    std::swap(first_index, temp.first_index);
    std::swap(last_index, temp.last_index);
    std::swap(number_of_elements, temp.number_of_elements);
    std::swap(arrays, temp.arrays);
    return *this;
  }
  Deque(int size) {
    int number = (size + array_size - 1) / array_size;
    if (number % 2 != 0) {
      ++number;
    }
    outer = new T*[number];
    int i = -1;
    int j = 0;
    try {
      while (i < number - 1) {
        ++i;
        T* newarr = reinterpret_cast<T*>(new char[array_size * sizeof(T)]);
        outer[i] = newarr;
        while (j < array_size) {
          new(outer[i] + j) T();
          ++j;
        }
        j = 0;
      }
    } catch (...) {
      --j;
      while (i >= 0) {
        while (j >= 0) {
          (outer[i] + j)->~T();
          --j;
        }
        delete[] reinterpret_cast<char*>(outer[i]);
        --i;
        j = array_size - 1;
      }
      delete[] outer;
      throw;
    }
    number_of_elements = size;
    arrays = number;
    int left;
    int right;
    if (size % 2 == 0) {
      left = size / 2;
      right = size / 2;
    } else {
      left = size / 2;
      right = size / 2 + 1;
    }
    first_arr_num = arrays / 2 - (left / array_size) - 1;
    last_arr_num = arrays / 2 + (right / array_size);
    first_arr = outer[first_arr_num];
    last_arr = outer[last_arr_num];
    first_index = array_size - 1 - left % array_size;
    last_index = right % array_size;
  }
  Deque(int size, const T& object) : Deque() {
    arrays = (size / array_size * 2) + 2;
    outer = new T*[arrays];
    int i = 0;
    try {
      while (i < arrays) {
        T* newarr = reinterpret_cast<T*>(new char[array_size * sizeof(T)]);
        outer[i] = newarr;
        ++i;
      }
    } catch (...) {
      for (int j = 0; j < i; ++j) {
        delete outer[j];
      }
      delete[] outer;
      throw;
    }
    while (size > 0) {
      push_back(object);
      --size;
      if (size > 0) {
        push_front(object);
        --size;
      }
    }
  }
  ~Deque() {
    for (iterator it = begin(); it < end(); ++it) {
      try {
        it->~T();
      } catch (...) {
        continue;
      }
    }
    for (int i = 0; i < arrays; ++i) {
      try {
        delete[] reinterpret_cast<char*>(outer[i]);
      } catch (...) {
        continue;
      }
    }
    try {
      delete[] outer;
    } catch (...) {}
  }
  void push_back(const T& TParam) {
    if (last_index == array_size) {
      if (last_arr_num == arrays - 1) {
        reallocate();
      }
      last_index = 0;
      ++last_arr_num;
    }
    last_arr = outer[last_arr_num];
    last_arr[last_index] = TParam;
    ++last_index;
    ++number_of_elements;
  };
  void push_front(const T& TParam) {
    if (first_index == -1) {
      if (first_arr_num == 0) {
        reallocate();
      }
      first_index = array_size - 1;
      --first_arr_num;
    }
    first_arr = outer[first_arr_num];
    first_arr[first_index] = TParam;
    --first_index;
    ++number_of_elements;
  };
  void reallocate() {
    arrays *= 2;
    T** newouter = new T*[arrays];
    for (int i = 0; i < arrays / 2; ++i) {
      newouter[i + arrays / 4] = outer[i];
    }
    int i = 0;
    int j = 3 * arrays / 4;
    try {
      while(i < arrays / 4) {
        T* newarr = reinterpret_cast<T*>(new char[array_size * sizeof(T)]);
        newouter[i] = newarr;
        ++i;
      }
    } catch(...) {
      for (size_t k = 0; k < i; ++k) {
        delete newouter[k];
      }
      throw;
    }
    try {
      while (j < arrays + 1) {
        T* newarr = reinterpret_cast<T*>(new char[array_size * sizeof(T)]);
        newouter[j] = newarr;
        ++j;
      }
    } catch (...) {
      for (size_t k = 3 * arrays / 4; k < j; ++k) {
        delete newouter[k];
      }
      for (size_t k = 0; k < i; ++ k) {
        delete newouter[k];
      }
      throw;
    }
    delete outer;
    outer = newouter;
    first_arr_num += (arrays / 4);
    last_arr_num += (arrays / 4);
    first_arr = outer[first_arr_num];
    last_arr = outer[last_arr_num];
  };
  void pop_back() {
    if (last_index != 0) {
      --last_index;
    } else {
      --last_arr_num;
      last_arr = outer[last_arr_num];
      last_index = array_size - 1;
    }
    last_arr[last_index].~T();
    --number_of_elements;
  };
  void pop_front() {
    if (first_index != array_size - 1) {
      ++first_index;
    } else {
      ++first_arr_num;
      first_arr = outer[first_arr_num];
      first_index = 0;
    }
    first_arr[first_index].~T();
    --number_of_elements;
  };
  T& operator[](size_t ind) {
    return *(begin() + ind);
  };
  const T& operator[](size_t ind) const {
    return *(cbegin() + ind);
  };
  T& at(size_t ind) {
    if (ind >= size()) {
      throw std::out_of_range("Index out of range");
    }
    return *(begin() + ind);
  }
  const T& at(size_t ind) const {
    if (ind >= size()) {
      throw std::out_of_range("Index out of range");
    }
    return *(cbegin() + ind);
  }
  size_t size() const {
    return number_of_elements;
  };

  template <bool is_const>
  struct base_iterator {
  public:
    using value_type = typename std::conditional_t<is_const, const T, T>;
    using iterator_category = std::random_access_iterator_tag;
    using const_iterator = base_iterator<true>;

    T** outer_array;
    T* obj_ptr;
    size_t array_index;
    size_t index;

    base_iterator(T** outer, size_t arr_ind, size_t ind) : outer_array(outer), array_index(arr_ind), index(ind) {
      obj_ptr = &(outer_array[array_index][index]);
    }

    value_type& operator*() const {
      return *obj_ptr;
    }
    value_type* operator->() const {
      return obj_ptr;
    }

    base_iterator& operator+=(int val) {
      if (val < 0) {
        return *this -= -val;
      }
      int a = val / array_size;
      int b = val % array_size;
      if (b + index > array_size - 1) {
        ++a;
        b -= array_size;
      }
      array_index += a;
      index += b;
      obj_ptr = &(outer_array[array_index][index]);
      return *this;
    }
    base_iterator& operator-=(int val) {
      if (val < 0) {
        return *this += -val;
      }
      int a = val / array_size;
      int b = val % array_size;
      if (static_cast<int>(index) < b) {
        ++a;
        b -= array_size;
      }
      array_index -= a;
      index -= b;
      obj_ptr = &(outer_array[array_index][index]);
      return *this;
    }
    base_iterator& operator++() {
      return *this += 1;
    }
    base_iterator operator++(int) {
      auto temp = *this;
      ++*this;
      return temp;
    }
    base_iterator& operator--() {
      return *this -= 1;
    }
    base_iterator operator--(int) {
      auto temp = *this;
      --*this;
      return temp;
    }
    base_iterator operator+(int val) const {
      base_iterator res = *this;
      res += val;
      return res;
    }
    base_iterator operator-(int val) const {
      base_iterator res = *this;
      res -= val;
      return res;
    }
    int operator-(const base_iterator& other) const {
      return (array_size * (array_index - other.array_index) - other.index + index);
    }
    bool operator<(const base_iterator& other) const {
      return (*this - other < 0);
    }
    bool operator<=(const base_iterator& other) const {
      return (*this - other <= 0);
    }
    bool operator>(const base_iterator& other) const {
      return (*this - other > 0);
    }
    bool operator>=(const base_iterator& other) const {
      return (*this - other >= 0);
    }
    bool operator==(const base_iterator& other) const {
      return (*this - other == 0);
    }
    bool operator!=(const base_iterator& other) const {
      return (*this - other != 0);
    }
    operator const_iterator() {
      return const_iterator(outer_array, array_index, index);
    }
  };

  using const_iterator = base_iterator<true>;
  using iterator = base_iterator<false>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  iterator begin() {
    size_t arr = first_arr_num;
    size_t ind = first_index;
    if (first_index == array_size - 1) {
      ++arr;
      ind = 0;
    } else {
      ++ind;
    }
    iterator iter(outer, arr, ind);
    return iter;
  }
  iterator end() {
    size_t arr = last_arr_num;
    size_t ind = last_index;
    iterator iter(outer, arr, ind);
    return iter;
  }
  const_iterator cbegin() const {
    size_t arr = first_arr_num;
    size_t ind = first_index;
    if (first_index == array_size - 1) {
      ++arr;
      ind = 0;
    } else {
      ++ind;
    }
    const_iterator iter(outer, arr, ind);
    return iter;
  }
  const_iterator begin() const {
    return cbegin();
  };
  const_iterator end() const {
    return cend();
  }
  const_iterator cend() const {
    size_t arr = last_arr_num;
    size_t ind = last_index;
    const_iterator iter(outer, arr, ind);
    return iter;
  }
  reverse_iterator rbegin() {
    return std::make_reverse_iterator(end());
  }
  reverse_iterator rend() {
    return std::make_reverse_iterator(begin());
  }
  const_reverse_iterator crbegin() const {
    return std::make_reverse_iterator(cend());
  }
  const_reverse_iterator crend() const {
    return std::make_reverse_iterator(cbegin());
  }

  void insert(iterator iter, const T& obj) {
    try {
      if (iter == end()) {
        push_back(obj);
        return;
      }
      push_back(obj);
      iterator iter1 = --(--end());
      iterator iter2 = --end();
      while (iter1 != iter) {
        std::swap(*iter1, *iter2);
        --iter1;
        --iter2;
      }
      std::swap(*iter1, *iter2);
    } catch(...) {
      --number_of_elements;
    }
  }
  void erase(iterator iter) {
    if (iter == end()) {
      pop_back();
      return;
    }
    iterator iter1 = iter + 1;
    while (iter1 != end()) {
      std::swap(*iter1, *iter);
      ++iter1;
      ++iter;
    }
    pop_back();
  }
};
