#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <numeric>
#include <iomanip>
#ifdef _WIN32
    #include <windows.h>
#endif

using namespace std;
using namespace chrono;

int input_integer(const int min_value = INT_MIN, const int max_value = INT_MAX) {
    int result;
    while (true) {
        if (cin >> result) {
            if (result < min_value)
                cout << "Число не может быть меньше " << min_value << '\n';
            else if (result > max_value)
                cout << "Число не может быть больше " << max_value << '\n';
            else
                return result;
        }
        else {
            cout << "Введены некорректные данные, ожидалось целое число\n";
        }
        cout << "Повторите ввод: ";
        cin.clear(); // Сброс флагов ошибок
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка буфера
    }
}

void insertion_sort(vector<int>& vec, const int low, const int high) {
    for (int i = low + 1; i <= high; ++i) {
        int key = vec[i];
        int j = i - 1;
        while (j >= low && vec[j] > key) {
            vec[j + 1] = vec[j];
            --j;
        }
        vec[j + 1] = key;
    }
}

void insertion_sort(vector<int>& vec) {
    insertion_sort(vec, 0, vec.size() - 1);
}

const int kInsertionSortThreshold = 16;

// Выбор опорного элемента для быстрой сортировки
int get_pivot_index(vector<int>& vec, const int low, const int high) {
    int mid = (low + high) / 2;
    if (vec[low] > vec[mid])
        swap(vec[low], vec[mid]);
    if (vec[mid] > vec[high])
        swap(vec[mid], vec[high]);
    if (vec[low] > vec[high])
        swap(vec[low], vec[high]);
    return mid;
}

// Разбиение Ломуто для быстрой сортировки
int lomuto_partition(vector<int>& vec, const int low, const int high) {
    int pivot_index = get_pivot_index(vec, low, high);
    swap(vec[pivot_index], vec[high]); // Перемещаем опорный элемент в конец

    int i = low;
    // Перемещаем элементы <= опорного в начало массива
    for (int j = low; j < high; ++j) {
        if (vec[j] <= vec[high])
            swap(vec[i++], vec[j]);
    }

    swap(vec[i], vec[high]); // Ставим опорный элемент на первую позицию после элементов <= его
    return i;
}

// Разбиение Хоара для быстрой сортировки
int hoare_partition(vector<int>& vec, const int low, const int high) {
    const int pivot = vec[get_pivot_index(vec, low, high)];
    int i = low, j = high;
    while (true) {
        while (vec[i] < pivot)
            ++i;
        while (vec[j] > pivot)
            --j;
        if (i >= j)
            return j;
        swap(vec[i++], vec[j--]);
    }
}

void quick_lomuto_sort(vector<int>& vec, const int low, const int high) {
    if (low < high) {
        int p = lomuto_partition(vec, low, high);
        quick_lomuto_sort(vec, low, p - 1);
        quick_lomuto_sort(vec, p + 1, high);
    }
}

void quick_lomuto_sort(vector<int>& vec) {
    quick_lomuto_sort(vec, 0, vec.size() - 1);
}

void quick_hoare_sort(vector<int>& vec, const int low, const int high) {
    if (low < high) {
        int p = hoare_partition(vec, low, high);
        quick_hoare_sort(vec, low, p);
        quick_hoare_sort(vec, p + 1, high);
    }
}

void quick_hoare_sort(vector<int>& vec) {
    quick_hoare_sort(vec, 0, vec.size() - 1);
}

void quick_optimized_sort(vector<int>& vec, const int low, const int high) {
    if (high - low <= kInsertionSortThreshold)
        return insertion_sort(vec, low, high);
    else {
        int p = hoare_partition(vec, low, high);
        quick_optimized_sort(vec, low, p);
        quick_optimized_sort(vec, p + 1, high);
    }
}

void quick_optimized_sort(vector<int>& vec) {
    quick_optimized_sort(vec, 0, vec.size() - 1);
}

// Слияние двух отсортированных подмассивов
void merge(vector<int>& vec, const int low, const int mid, const int high) {
    const vector<int> leftArr(vec.begin() + low, vec.begin() + mid + 1);
    const vector<int> rightArr(vec.begin() + mid + 1, vec.begin() + high + 1);

    int i = 0, j = 0, k = low; // Индексы каждого из массивов

    // Слияние до конца одного из массивов
    for (; i < leftArr.size() && j < rightArr.size(); ++k) {
        if (leftArr[i] <= rightArr[j])
            vec[k] = leftArr[i++];
        else
            vec[k] = rightArr[j++];
    }

    // Копируем оставшиеся элементы из левого массива
    for (; i < leftArr.size(); ++i, ++k)
        vec[k] = leftArr[i];

    // Копируем оставшиеся элементы из правого массива
    for (; j < rightArr.size(); ++j, ++k)
        vec[k] = rightArr[j];
}

void merge_sort(vector<int>& vec, const int low, const int high) {
    if (low < high) {
        // Делим на 2 отсортированные части
        int mid = (low + high) / 2;
        merge_sort(vec, low, mid);
        merge_sort(vec, mid + 1, high);
        // Сливаем в 1 массив
        merge(vec, low, mid, high);
    }
}

void merge_sort(vector<int>& vec) {
    merge_sort(vec, 0, vec.size() - 1);
}

// Преобразование в двоичную кучу поддерева с корнем i
void heapify(vector<int>& vec, const int n, const int i) {
    int largest = i;       // Корень - наибольший элемент
    const int left = 2 * i + 1, right = 2 * i + 2; // Дети

    // Левый дочерний элемент больше корня
    if (left < n && vec[left] > vec[largest])
        largest = left;

    // Правый дочерний элемент наибольший
    if (right < n && vec[right] > vec[largest])
        largest = right;

    // Самый большой элемент - ребёнок
    if (largest != i) {
        swap(vec[i], vec[largest]);
        heapify(vec, n, largest);
    }
}

void heap_sort(vector<int>& vec) {
    // Построение кучи (перегруппируем массив)
    for (int i = vec.size() / 2 - 1; i >= 0; --i)
        heapify(vec, vec.size(), i);

    // Один за другим извлекаем элементы из кучи
    for (int i = vec.size() - 1; i >= 0; --i) {
        // Перемещаем текущий корень в конец
        swap(vec[0], vec[i]);

        // Вызываем процедуру heapify на уменьшенной куче
        heapify(vec, i, 0);
    }
}

void std_sort(vector<int>& vec) {
    sort(vec.begin(), vec.end());
}



// Измерение времени сортировки
long long measure_sort_time(void (* const sort_function)(vector<int>&), const vector<int>& vec, const vector<int>& ans, const int runs = 5) {
    vector<long long> times;

    for (int i = 0; i < runs; ++i) {
        vector<int> vec_copy = vec;

        auto start = high_resolution_clock::now();
        sort_function(vec_copy);
        if (!is_sorted(vec_copy.begin(), vec_copy.end()))
            cerr << "Ошибка! Массив не отсортирован!\n";
        else if (vec_copy != ans)
            cerr << "Ошибка! Массив изменён!\n";
        auto end = high_resolution_clock::now();

        auto duration = duration_cast<microseconds>(end - start).count();
        times.push_back(duration);
    }

    // Находим медианное время для этого теста
    sort(times.begin(), times.end());
    return times[times.size() / 2];
}

vector<vector<int>> test_cases, test_answers;

// Генерация тестовых данных
void generateTestCases(int n) {
    random_device rd;
    mt19937 gen(rd());

    // Отсортированный массив
    vector<int> sorted(n);
    iota(sorted.begin(), sorted.end(), 0);
    test_cases.push_back(sorted);

    // Массив, отсортированный в обратном порядке
    vector<int> reversed(n);
    iota(reversed.begin(), reversed.end(), 0);
    reverse(reversed.begin(), reversed.end());
    test_cases.push_back(reversed);

    // Массив со всеми одинаковыми элементами
    vector<int> all_same(n, 13);
    test_cases.push_back(all_same);

    // 10 случайных массивов
    uniform_int_distribution<> dist(0, min((long long) INT_MAX, n * 10ll));
    for (int i = 0; i < 10; ++i) {
        vector<int> random(n);
        for (int j = 0; j < n; ++j)
            random[j] = dist(gen);
        test_cases.push_back(random);
    }
}

// Генерация ответов на тесты
void generateTestAnswers() {
    test_answers = test_cases;
    for (int i = 0; i < test_cases.size(); ++i)
        sort(test_answers[i].begin(), test_answers[i].end());
}

string format_time(const long long time) {
    long double value;
    string unit;
    if (time < 1000) {
        value = time;
        unit = "мкс";
    }
    else if (time < 1'000'000) {
        value = time / 1000.0l;
        unit = "мс";
    }
    else {
        value = time / 1'000'000.0l;
        unit = "с";
    }

    int precision;
    if (value < 10) precision = 2;
    else if (value < 100) precision = 1;
    else precision = 0;

    ostringstream oss;
    oss << fixed << setprecision(precision) << value << ' ' << unit;
    return oss.str();
}

const vector<string> sort_names = {
    "Сортировка вставками",
    "Сортировка слиянием",
    "Быстрая сортировка c разбиением Ломуто",
    "Быстрая сортировка cо схемой Хоара",
    "Быстрая оптимизированная сортировка (c переключением на вставки)",
    "Пирамидальная сортировка",
    "Встроенная сортировка (std::sort)"
};

const vector<void(*)(vector<int>&)> sort_functions = {
    insertion_sort,
    merge_sort,
    quick_lomuto_sort,
    quick_hoare_sort,
    quick_optimized_sort,
    heap_sort,
    std_sort
};

const vector<int> max_valid_lengths = {
    25'000,
    500'000,
    3'500,
    5'000'000,
    5'000'000,
    2'000'000,
    5'000'000
};

const int sort_count = sort_names.size();

const vector<string> test_names = {
    "Отсортированный",
    "Обратный порядок",
    "Все одинаковые",
    "Случайный 1",
    "Случайный 2",
    "Случайный 3",
    "Случайный 4",
    "Случайный 5",
    "Случайный 6",
    "Случайный 7",
    "Случайный 8",
    "Случайный 9",
    "Случайный 10"
};

const int test_count = test_names.size();

int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    #endif
    cout << "Введите размер массива: ";

    const int n = input_integer(0, *max_element(max_valid_lengths.begin(), max_valid_lengths.end()));
    vector<int> vec(n);

    cout << "Генерация тестов...\n";
    generateTestCases(n);
    generateTestAnswers();
    cout << "Генерация тестов завершена\n";

    for (int i = 0; i < sort_count; ++i) {
        cout << "Тестируется " << sort_names[i] << '\n';
        if (n > max_valid_lengths[i]) {
            cout << "Для n = " << n << " будет выполняться слишком долго\n\n";
            continue;
        }
        vector<long long> all_times;
        for (int j = 0; j < test_count; ++j) {
            long long time = measure_sort_time(sort_functions[i], test_cases[j], test_answers[j]);
            all_times.push_back(time);
            cout << test_names[j] << ": " << format_time(time) << '\n';
        }

        long long min_time = *min_element(all_times.begin(), all_times.end());
        long long max_time = *max_element(all_times.begin(), all_times.end());
        long long average_time_random = accumulate(all_times.begin() + 3, all_times.end(), 0ll) / (all_times.size() - 3);
        cout << "Среднее время на случайных тестах: " << format_time(average_time_random) << '\n';
        cout << "Лучшее время: " << format_time(min_time) << '\n';
        cout << "Худшее время: " << format_time(max_time) << '\n';
        cout << '\n';
    }
}
