//
// main.cc
//
// Main method to run all performance tests
//
// Authors : Gokcehan Kara <gokcehankara@gmail.com>
// License : This file is placed in the public domain.
//

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <random>
#include <string>

#include <json/json.h>

#include "DequeConservative.h"
#include "DequeNaive.h"
#include "DequeReclaiming.h"
#include "QueueConservative.h"
#include "QueueNaive.h"
#include "QueueReclaiming.h"
#include "StdDeque.h"
#include "StdList.h"
#include "StdVector.h"

struct Small {
    int data[1];
    bool operator<(const Small& that)
    {
        return this->data[0] < that.data[0];
    }
};

struct Medium {
    int data[10];
    bool operator<(const Medium& that)
    {
        return this->data[0] < that.data[0];
    }
};

struct Large {
    int data[100];
    bool operator<(const Large& that)
    {
        return this->data[0] < that.data[0];
    }
};

template <typename> static Json::Value fill_back_test(const size_t);
template <typename> static Json::Value fill_back_reserved_test(const size_t);
template <typename> static Json::Value fill_front_test(const size_t);
template <typename> static Json::Value fill_front_reserved_test(const size_t);
template <typename> static Json::Value queue_test(const size_t);
template <typename> static Json::Value zigzag_test(const size_t);
template <typename> static Json::Value traverse_test(const size_t);
template <typename> static Json::Value shuffle_test(const size_t);
template <typename> static Json::Value qsort_test(const size_t);

template <typename> static Json::Value fill_back_memory_test(const size_t);
template <typename> static Json::Value fill_front_memory_test(const size_t);
template <typename> static Json::Value queue_memory_test(const size_t);
template <typename> static Json::Value zigzag_memory_test(const size_t);

static void print_test_data(std::ofstream& out,
                            std::vector<std::string>& test_names,
                            const std::string& test_name,
                            const Json::Value& test_data)
{
    test_names.push_back(test_name);
    std::string str = test_data.toStyledString();
    str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
    out << "var json" << test_name << " = " << str << ";" << std::endl;
    out << "var data" << test_name << " = new google.visualization.DataTable(json"
        << test_name << ");" << std::endl;
    out << "var chart" << test_name
        << " = new google.visualization.LineChart(document.getElementById('div"
        << test_name << "'));" << std::endl;
    out << "chart" << test_name << ".draw(data" << test_name << ", {"
        << "title: '" << test_name << "', "
        << "backgroundColor: {fill:'transparent'}, "
        << "vAxis: {title: 'time (ms)'}, "
        << "hAxis: {title: 'n'}, "
        << "width: 'auto', "
        << "height: 500});" << std::endl;
    out << std::endl;
}

static void print_memory_test_data(std::ofstream& out,
                                   std::vector<std::string>& test_names,
                                   const std::string& test_name,
                                   const Json::Value& test_data)
{
    test_names.push_back(test_name);
    std::string str = test_data.toStyledString();
    str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
    out << "var json" << test_name << " = " << str << ";" << std::endl;
    out << "var data" << test_name << " = new google.visualization.DataTable(json"
        << test_name << ");" << std::endl;
    out << "var chart" << test_name
        << " = new google.visualization.LineChart(document.getElementById('div"
        << test_name << "'));" << std::endl;
    out << "chart" << test_name << ".draw(data" << test_name << ", {"
        << "title: '" << test_name << "', "
        << "backgroundColor: {fill:'transparent'}, "
        << "vAxis: {title: 'load', minValue: 0, maxValue: 1}, "
        << "hAxis: {title: 'n'}, "
        << "width: 'auto', "
        << "height: 500});" << std::endl;
    out << std::endl;
}

static void run_tests_and_print_html(std::ofstream& out)
{
    out << " <html>" << std::endl;
    out << "   <head>" << std::endl;
    out << "     <!--Load the AJAX API-->" << std::endl;
    out << "     <script type='text/javascript' "
           "src='https://www.google.com/jsapi'></script>" << std::endl;
    out << "     <script type='text/javascript' "
           "src='//ajax.googleapis.com/ajax/libs/jquery/1.10.2/jquery.min.js'></"
           "script>" << std::endl;
    out << "     <script type='text/javascript'>" << std::endl;
    out << std::endl;
    out << "     google.load('visualization', '1', {'packages':['corechart']});"
        << std::endl;
    out << std::endl;
    out << "     google.setOnLoadCallback(drawChart);" << std::endl;
    out << std::endl;
    out << "     function drawChart() {" << std::endl;

    // run tests and print data
    size_t n = 1000;
    std::vector<std::string> test_names;
    print_test_data(out, test_names, "FillBackSmall"          , fill_back_test<Small>(100 * n));
    print_test_data(out, test_names, "FillBackMedium"         , fill_back_test<Medium>(100 * n));
    print_test_data(out, test_names, "FillBackLarge"          , fill_back_test<Large>(10 * n));
    print_test_data(out, test_names, "FillBackReservedSmall"  , fill_back_reserved_test<Small>(100 * n));
    print_test_data(out, test_names, "FillBackReservedMedium" , fill_back_reserved_test<Medium>(100 * n));
    print_test_data(out, test_names, "FillBackReservedLarge"  , fill_back_reserved_test<Large>(10 * n));
    print_test_data(out, test_names, "FillFrontSmall"         , fill_front_test<Small>(100 * n));
    print_test_data(out, test_names, "FillFrontMedium"        , fill_front_test<Medium>(100 * n));
    print_test_data(out, test_names, "FillFrontLarge"         , fill_front_test<Large>(10 * n));
    print_test_data(out, test_names, "FillFrontReservedSmall" , fill_front_reserved_test<Small>(100 * n));
    print_test_data(out, test_names, "FillFrontReservedMedium", fill_front_reserved_test<Medium>(100 * n));
    print_test_data(out, test_names, "FillFrontReservedLarge" , fill_front_reserved_test<Large>(10 * n));
    print_test_data(out, test_names, "QueueSmall"             , queue_test<Small>(100 * n));
    print_test_data(out, test_names, "QueueMedium"            , queue_test<Medium>(100 * n));
    print_test_data(out, test_names, "QueueLarge"             , queue_test<Large>(10 * n));
    print_test_data(out, test_names, "ZigzagSmall"            , zigzag_test<Small>(100 * n));
    print_test_data(out, test_names, "ZigzagMedium"           , zigzag_test<Medium>(100 * n));
    print_test_data(out, test_names, "ZigzagLarge"            , zigzag_test<Large>(10 * n));
    print_test_data(out, test_names, "TraverseSmall"          , traverse_test<Small>(100 * n));
    print_test_data(out, test_names, "TraverseMedium"         , traverse_test<Medium>(100 * n));
    print_test_data(out, test_names, "TraverseLarge"          , traverse_test<Large>(10 * n));
    print_test_data(out, test_names, "ShuffleSmall"           , shuffle_test<Small>(100 * n));
    print_test_data(out, test_names, "ShuffleMedium"          , shuffle_test<Medium>(100 * n));
    print_test_data(out, test_names, "ShuffleLarge"           , shuffle_test<Large>(10 * n));
    print_test_data(out, test_names, "QSortSmall"             , qsort_test<Small>(10 * n));
    print_test_data(out, test_names, "QSortMedium"            , qsort_test<Medium>(10 * n));
    print_test_data(out, test_names, "QSortLarge"             , qsort_test<Large>(10 * n));

    print_memory_test_data(out, test_names, "FillBackMemory"  , fill_back_memory_test<Small>(100 * n));
    print_memory_test_data(out, test_names, "FillFrontMemory" , fill_front_memory_test<Small>(100 * n));
    print_memory_test_data(out, test_names, "QueueMemory"     , queue_memory_test<Small>(100 * n));
    print_memory_test_data(out, test_names, "ZigzagMemory"    , zigzag_memory_test<Small>(100 * n));

    out << "    }" << std::endl;
    out << std::endl;
    out << "    </script>" << std::endl;
    out << "  </head>" << std::endl;
    out << std::endl;
    out << "  <body>" << std::endl;

    // print divs for graphs
    for (const auto test_name : test_names) {
        out << "<div id='div" << test_name << "'></div>" << std::endl;
    }

    out << "  </body>" << std::endl;
    out << "</html>" << std::endl;
}

static Json::Value get_json_template()
{
    Json::Value res;

    res["cols"][0]["id"] = "n";
    res["cols"][0]["label"] = "N";
    res["cols"][0]["type"] = "string";

    res["cols"][1]["id"] = "stdvector";
    res["cols"][1]["label"] = "StdVector";
    res["cols"][1]["type"] = "number";

    res["cols"][2]["id"] = "stddeque";
    res["cols"][2]["label"] = "StdDeque";
    res["cols"][2]["type"] = "number";

    res["cols"][3]["id"] = "stdlist";
    res["cols"][3]["label"] = "StdList";
    res["cols"][3]["type"] = "number";

    res["cols"][4]["id"] = "queuenaive";
    res["cols"][4]["label"] = "QueueNaive";
    res["cols"][4]["type"] = "number";

    res["cols"][5]["id"] = "queuereclaiming";
    res["cols"][5]["label"] = "QueueReclaiming";
    res["cols"][5]["type"] = "number";

    res["cols"][6]["id"] = "queueconservative";
    res["cols"][6]["label"] = "QueueConservative";
    res["cols"][6]["type"] = "number";

    res["cols"][7]["id"] = "dequenaive";
    res["cols"][7]["label"] = "DequeNaive";
    res["cols"][7]["type"] = "number";

    res["cols"][8]["id"] = "dequereclaiming";
    res["cols"][8]["label"] = "DequeReclaiming";
    res["cols"][8]["type"] = "number";

    res["cols"][9]["id"] = "dequeconservative";
    res["cols"][9]["label"] = "DequeConservative";
    res["cols"][9]["type"] = "number";

    return res;
}

template <typename Function>
static double get_average(Function f, const size_t n)
{
    std::vector<double> timings;
    for (int i = 0; i < 5; ++i) {
        timings.push_back(f(n));
    }

    // average of 3 runs out of 5 runs total exluding the best and the worst
    std::sort(timings.begin(), timings.end());
    return (timings[1] + timings[2] + timings[3] / 3.0);
}

template <template <typename> class Container, typename Type>
static double fill_back(const size_t n)
{
    using namespace std::chrono;
    Container<Type> c;

    auto t1 = high_resolution_clock::now();

    for (size_t i = 0; i < n; ++i) {
        c.push_back(Type());
    }

    auto t2 = high_resolution_clock::now();
    auto time_span = duration_cast<duration<double, std::milli>>(t2 - t1);

    return time_span.count();
}

template <template <typename> class Container, typename Type>
static double fill_back_memory(const size_t n)
{
    double total = 0;
    Container<Type> c;
    for (size_t i = 0; i < n; ++i) {
        c.push_back(Type());
        total += c.load_factor();
    }
    return total / n;
}

template <template <typename> class Container, typename Type>
static double fill_back_reserved(const size_t n)
{
    using namespace std::chrono;
    Container<Type> c;

    auto t1 = high_resolution_clock::now();

    c.reserve_back(n);
    for (size_t i = 0; i < n; ++i) {
        c.push_back(Type());
    }

    auto t2 = high_resolution_clock::now();
    auto time_span = duration_cast<duration<double, std::milli>>(t2 - t1);

    return time_span.count();
}

template <template <typename> class Container, typename Type>
static double fill_front(const size_t n)
{
    using namespace std::chrono;
    Container<Type> c;

    auto t1 = high_resolution_clock::now();

    for (size_t i = 0; i < n; ++i) {
        c.push_front(Type());
    }

    auto t2 = high_resolution_clock::now();
    auto time_span = duration_cast<duration<double, std::milli>>(t2 - t1);

    return time_span.count();
}

template <template <typename> class Container, typename Type>
static double fill_front_memory(const size_t n)
{
    double total = 0;
    Container<Type> c;
    for (size_t i = 0; i < n; ++i) {
        c.push_front(Type());
        total += c.load_factor();
    }
    return total / n;
}

template <template <typename> class Container, typename Type>
static double fill_front_reserved(const size_t n)
{
    using namespace std::chrono;
    Container<Type> c;

    auto t1 = high_resolution_clock::now();

    c.reserve_front(n);
    for (size_t i = 0; i < n; ++i) {
        c.push_front(Type());
    }

    auto t2 = high_resolution_clock::now();
    auto time_span = duration_cast<duration<double, std::milli>>(t2 - t1);

    return time_span.count();
}

template <template <typename> class Container, typename Type>
static double queue(const size_t n)
{
    using namespace std::chrono;
    const int load = 1000;
    Container<Type> c;
    for (size_t i = 0; i < load; ++i) {
        c.push_back(Type());
    }

    auto t1 = high_resolution_clock::now();

    for (size_t i = 0; i < n; ++i) {
        if (i % 2 == 0) {
            c.push_back(Type());
        } else {
            c.pop_front();
        }
    }

    auto t2 = high_resolution_clock::now();
    auto time_span = duration_cast<duration<double, std::milli>>(t2 - t1);

    return time_span.count();
}

template <template <typename> class Container, typename Type>
static double queue_memory(const size_t n)
{
    double total = 0;
    const int load = 1000;
    Container<Type> c;
    for (size_t i = 0; i < load; ++i) {
        c.push_back(Type());
    }
    for (size_t i = 0; i < n; ++i) {
        if (i % 2 == 0) {
            c.push_back(Type());
        } else {
            c.pop_front();
        }
        total += c.load_factor();
    }
    return total / n;
}

template <template <typename> class Container, typename Type>
static double zigzag(const size_t n) {
    using namespace std::chrono;
    Container<Type> c;

    auto t1 = high_resolution_clock::now();

    for (size_t i = 0; i < n / 4; ++i) {
        c.push_back(Type());
    }
    for (size_t i = 0; i < n / 4; ++i) {
        c.push_front(Type());
    }
    for (size_t i = 0; i < n / 2; ++i) {
        c.pop_back();
    }
    for (size_t i = 0; i < n / 4; ++i) {
        c.push_front(Type());
    }
    for (size_t i = 0; i < n / 4; ++i) {
        c.push_back(Type());
    }
    for (size_t i = 0; i < n / 2; ++i) {
        c.pop_front();
    }

    auto t2 = high_resolution_clock::now();
    auto time_span = duration_cast<duration<double, std::milli>>(t2 - t1);

    return time_span.count();
}

template <template <typename> class Container, typename Type>
static double zigzag_memory(const size_t n)
{
    double total = 0;
    Container<Type> c;

    for (size_t i = 0; i < n / 4; ++i) {
        c.push_back(Type());
        total += c.load_factor();
    }
    for (size_t i = 0; i < n / 4; ++i) {
        c.push_front(Type());
        total += c.load_factor();
    }
    for (size_t i = 0; i < n / 2; ++i) {
        c.pop_back();
        total += c.load_factor();
    }
    for (size_t i = 0; i < n / 4; ++i) {
        c.push_front(Type());
        total += c.load_factor();
    }
    for (size_t i = 0; i < n / 4; ++i) {
        c.push_back(Type());
        total += c.load_factor();
    }
    for (size_t i = 0; i < n / 2; ++i) {
        c.pop_front();
        total += c.load_factor();
    }

    return total / (2 * n);
}

template <template <typename> class Container, typename Type>
static double traverse(const size_t n)
{
    using namespace std::chrono;
    Container<Type> c;
    for (size_t i = 0; i < n; ++i) {
        c.push_back(Type());
    }

    auto t1 = high_resolution_clock::now();

    for (size_t i = 0; i < n; ++i) {
        c[i].data[0]++;
    }

    auto t2 = high_resolution_clock::now();
    auto time_span = duration_cast<duration<double, std::milli>>(t2 - t1);

    return time_span.count();
}

template <template <typename> class Container, typename Type>
static double shuffle(const size_t n)
{
    using namespace std::chrono;
    Container<Type> c;
    for (size_t i = 0; i < n; ++i) {
        c.push_back(Type());
    }
    std::default_random_engine generator;
    std::uniform_int_distribution<size_t> distribution(0, n - 1);
    auto rand = std::bind(distribution, generator);

    auto t1 = high_resolution_clock::now();

    for (size_t i = n - 1; i > 0; --i) {
        std::swap(c[rand()], c[i]);
    }

    auto t2 = high_resolution_clock::now();
    auto time_span = duration_cast<duration<double, std::milli>>(t2 - t1);

    return time_span.count();
}

template <typename Container, typename Type>
void qsort_helper(Container& arr, const size_t left, const size_t right)
{
    if (left < right && right + 1 != 0) {
        Type pivot = arr[right];
        size_t index = left;
        for (size_t i = left; i < right; ++i) {
            if (arr[i] < pivot) {
                std::swap(arr[i], arr[index]);
                index++;
            }
        }
        arr[right] = arr[index];
        arr[index] = pivot;
        qsort_helper<Container, Type>(arr, left, index - 1);
        qsort_helper<Container, Type>(arr, index + 1, right);
    }
}

template <template <typename> class Container, typename Type>
static double qsort(const size_t n)
{
    using namespace std::chrono;
    Container<Type> c;
    for (size_t i = 0; i < n; ++i) {
        Type t;
        t.data[0] = static_cast<int>(i);
        c.push_back(t);
    }
    std::default_random_engine generator;
    std::uniform_int_distribution<size_t> distribution(0, n - 1);
    auto rand = std::bind(distribution, generator);

    for (size_t i = n - 1; i > 0; --i) {
        std::swap(c[rand()], c[i]);
    }

    auto t1 = high_resolution_clock::now();

    qsort_helper<Container<Type>, Type>(c, 0, n - 1);

    auto t2 = high_resolution_clock::now();
    auto time_span = duration_cast<duration<double, std::milli>>(t2 - t1);

    return time_span.count();
}

template <typename Type>
static Json::Value fill_back_test(const size_t step)
{
    std::cout << "fill-back-test<" << typeid(Type).name() << "> " << std::flush;
    Json::Value res = get_json_template();

    for (int i = 0; i < 10; ++i) {
        size_t n = static_cast<size_t>(i + 1) * step;
        res["rows"][i]["c"][0]["v"] = std::to_string(n);

        get_average(fill_back<StdDeque, Type>, n); // warm-up

        res["rows"][i]["c"][1]["v"] = get_average(fill_back<StdVector, Type>, n);          std::cout << "." << std::flush;
        res["rows"][i]["c"][2]["v"] = get_average(fill_back<StdDeque, Type>, n);           std::cout << "." << std::flush;
        res["rows"][i]["c"][3]["v"] = get_average(fill_back<StdList, Type>, n);            std::cout << "." << std::flush;
        res["rows"][i]["c"][4]["v"] = get_average(fill_back<QueueNaive, Type>, n);         std::cout << "." << std::flush;
        res["rows"][i]["c"][5]["v"] = get_average(fill_back<QueueReclaiming, Type>, n);    std::cout << "." << std::flush;
        res["rows"][i]["c"][6]["v"] = get_average(fill_back<QueueConservative, Type>, n);  std::cout << "." << std::flush;
        res["rows"][i]["c"][7]["v"] = get_average(fill_back<DequeNaive, Type>, n);         std::cout << "." << std::flush;
        res["rows"][i]["c"][8]["v"] = get_average(fill_back<DequeReclaiming, Type>, n);    std::cout << "." << std::flush;
        res["rows"][i]["c"][9]["v"] = get_average(fill_back<DequeConservative, Type>, n);  std::cout << "." << std::flush;
    }

    std::cout << " DONE" << std::endl;
    return res;
}

template <typename Type>
static Json::Value fill_back_memory_test(const size_t step)
{
    std::cout << "fill-back-memory-test<" << typeid(Type).name() << "> " << std::flush;
    Json::Value res = get_json_template();

    for (int i = 0; i < 10; ++i) {
        size_t n = static_cast<size_t>(i + 1) * step;
        res["rows"][i]["c"][0]["v"] = std::to_string(n);

        res["rows"][i]["c"][1]["v"] = fill_back_memory<StdVector, Type>(n);          std::cout << "." << std::flush;
        res["cols"][2]["label"] = "(NR)StdDeque";
        res["cols"][3]["label"] = "(NR)StdList";
        res["rows"][i]["c"][4]["v"] = fill_back_memory<QueueNaive, Type>(n);         std::cout << "." << std::flush;
        res["rows"][i]["c"][5]["v"] = fill_back_memory<QueueReclaiming, Type>(n);    std::cout << "." << std::flush;
        res["rows"][i]["c"][6]["v"] = fill_back_memory<QueueConservative, Type>(n);  std::cout << "." << std::flush;
        res["rows"][i]["c"][7]["v"] = fill_back_memory<DequeNaive, Type>(n);         std::cout << "." << std::flush;
        res["rows"][i]["c"][8]["v"] = fill_back_memory<DequeReclaiming, Type>(n);    std::cout << "." << std::flush;
        res["rows"][i]["c"][9]["v"] = fill_back_memory<DequeConservative, Type>(n);  std::cout << "." << std::flush;
    }

    std::cout << " DONE" << std::endl;
    return res;
}

template <typename Type>
static Json::Value fill_back_reserved_test(const size_t step)
{
    std::cout << "fill-back-reserved-test<" << typeid(Type).name() << "> " << std::flush;
    Json::Value res = get_json_template();

    for (int i = 0; i < 10; ++i) {
        size_t n = static_cast<size_t>(i + 1) * step;
        res["rows"][i]["c"][0]["v"] = std::to_string(n);

        get_average(fill_back<StdDeque, Type>, n); // warm-up

        res["rows"][i]["c"][1]["v"] = get_average(fill_back_reserved<StdVector, Type>, n);          std::cout << "." << std::flush;
        res["rows"][i]["c"][2]["v"] = get_average(fill_back<StdDeque, Type>, n);                    std::cout << "." << std::flush;
        res["rows"][i]["c"][3]["v"] = get_average(fill_back<StdList, Type>, n);                     std::cout << "." << std::flush;
        res["rows"][i]["c"][4]["v"] = get_average(fill_back_reserved<QueueNaive, Type>, n);         std::cout << "." << std::flush;
        res["rows"][i]["c"][5]["v"] = get_average(fill_back_reserved<QueueReclaiming, Type>, n);    std::cout << "." << std::flush;
        res["rows"][i]["c"][6]["v"] = get_average(fill_back_reserved<QueueConservative, Type>, n);  std::cout << "." << std::flush;
        res["rows"][i]["c"][7]["v"] = get_average(fill_back_reserved<DequeNaive, Type>, n);         std::cout << "." << std::flush;
        res["rows"][i]["c"][8]["v"] = get_average(fill_back_reserved<DequeReclaiming, Type>, n);    std::cout << "." << std::flush;
        res["rows"][i]["c"][9]["v"] = get_average(fill_back_reserved<DequeConservative, Type>, n);  std::cout << "." << std::flush;
    }

    std::cout << " DONE" << std::endl;
    return res;
}

template <typename Type>
static Json::Value fill_front_test(const size_t step)
{
    std::cout << "fill-front-test<" << typeid(Type).name() << "> " << std::flush;
    Json::Value res = get_json_template();

    for (int i = 0; i < 10; ++i) {
        size_t n = static_cast<size_t>(i + 1) * step;
        res["rows"][i]["c"][0]["v"] = std::to_string(n);

        get_average(fill_front<StdDeque, Type>, n); // warm-up

        res["cols"][1]["label"] = "(NR)StdVector";
        res["rows"][i]["c"][2]["v"] = get_average(fill_front<StdDeque, Type>, n);           std::cout << "." << std::flush;
        res["rows"][i]["c"][3]["v"] = get_average(fill_front<StdList, Type>, n);            std::cout << "." << std::flush;
        res["cols"][4]["label"] = "(NR)QueueNaive";
        res["cols"][5]["label"] = "(NR)QueueReclaiming";
        res["cols"][6]["label"] = "(NR)QueueConservative";
        res["rows"][i]["c"][7]["v"] = get_average(fill_front<DequeNaive, Type>, n);         std::cout << "." << std::flush;
        res["rows"][i]["c"][8]["v"] = get_average(fill_front<DequeReclaiming, Type>, n);    std::cout << "." << std::flush;
        res["rows"][i]["c"][9]["v"] = get_average(fill_front<DequeConservative, Type>, n);  std::cout << "." << std::flush;
    }

    std::cout << " DONE" << std::endl;
    return res;
}

template <typename Type>
static Json::Value fill_front_memory_test(const size_t step)
{
    std::cout << "fill-front-memory-test<" << typeid(Type).name() << "> " << std::flush;
    Json::Value res = get_json_template();

    for (int i = 0; i < 10; ++i) {
        size_t n = static_cast<size_t>(i + 1) * step;
        res["rows"][i]["c"][0]["v"] = std::to_string(n);

        res["cols"][1]["label"] = "(NR)StdVector";
        res["cols"][2]["label"] = "(NR)StdDeque";
        res["cols"][3]["label"] = "(NR)StdList";
        res["cols"][4]["label"] = "(NR)QueueNaive";
        res["cols"][5]["label"] = "(NR)QueueReclaiming";
        res["cols"][6]["label"] = "(NR)QueueConservative";
        res["rows"][i]["c"][7]["v"] = fill_front_memory<DequeNaive, Type>(n);         std::cout << "." << std::flush;
        res["rows"][i]["c"][8]["v"] = fill_front_memory<DequeReclaiming, Type>(n);    std::cout << "." << std::flush;
        res["rows"][i]["c"][9]["v"] = fill_front_memory<DequeConservative, Type>(n);  std::cout << "." << std::flush;
    }

    std::cout << " DONE" << std::endl;
    return res;
}

template <typename Type>
static Json::Value fill_front_reserved_test(const size_t step)
{
    std::cout << "fill-front-reserved-test<" << typeid(Type).name() << "> " << std::flush;
    Json::Value res = get_json_template();

    for (int i = 0; i < 10; ++i) {
        size_t n = static_cast<size_t>(i + 1) * step;
        res["rows"][i]["c"][0]["v"] = std::to_string(n);

        get_average(fill_front<StdDeque, Type>, n); // warm-up

        res["cols"][1]["label"] = "(NR)StdVector";
        res["rows"][i]["c"][2]["v"] = get_average(fill_front<StdDeque, Type>, n);                    std::cout << "." << std::flush;
        res["rows"][i]["c"][3]["v"] = get_average(fill_front<StdList, Type>, n);                     std::cout << "." << std::flush;
        res["rows"][i]["c"][4]["v"] = get_average(fill_front_reserved<QueueNaive, Type>, n);         std::cout << "." << std::flush;
        res["rows"][i]["c"][5]["v"] = get_average(fill_front_reserved<QueueReclaiming, Type>, n);    std::cout << "." << std::flush;
        res["rows"][i]["c"][6]["v"] = get_average(fill_front_reserved<QueueConservative, Type>, n);  std::cout << "." << std::flush;
        res["rows"][i]["c"][7]["v"] = get_average(fill_front_reserved<DequeNaive, Type>, n);         std::cout << "." << std::flush;
        res["rows"][i]["c"][8]["v"] = get_average(fill_front_reserved<DequeReclaiming, Type>, n);    std::cout << "." << std::flush;
        res["rows"][i]["c"][9]["v"] = get_average(fill_front_reserved<DequeConservative, Type>, n);  std::cout << "." << std::flush;
    }

    std::cout << " DONE" << std::endl;
    return res;
}

template <typename Type>
static Json::Value queue_test(const size_t step)
{
    std::cout << "queue-test<" << typeid(Type).name() << "> " << std::flush;
    Json::Value res = get_json_template();

    for (int i = 0; i < 10; ++i) {
        size_t n = static_cast<size_t>(i + 1) * step;
        res["rows"][i]["c"][0]["v"] = std::to_string(n);

        get_average(queue<StdDeque, Type>, n); // warm-up

        res["cols"][1]["label"] = "(NR)StdVector";
        res["rows"][i]["c"][2]["v"] = get_average(queue<StdDeque, Type>, n);           std::cout << "." << std::flush;
        res["rows"][i]["c"][3]["v"] = get_average(queue<StdList, Type>, n);            std::cout << "." << std::flush;
        res["rows"][i]["c"][4]["v"] = get_average(queue<QueueNaive, Type>, n);         std::cout << "." << std::flush;
        res["rows"][i]["c"][5]["v"] = get_average(queue<QueueReclaiming, Type>, n);    std::cout << "." << std::flush;
        res["rows"][i]["c"][6]["v"] = get_average(queue<QueueConservative, Type>, n);  std::cout << "." << std::flush;
        res["rows"][i]["c"][7]["v"] = get_average(queue<DequeNaive, Type>, n);         std::cout << "." << std::flush;
        res["rows"][i]["c"][8]["v"] = get_average(queue<DequeReclaiming, Type>, n);    std::cout << "." << std::flush;
        res["rows"][i]["c"][9]["v"] = get_average(queue<DequeConservative, Type>, n);  std::cout << "." << std::flush;
    }

    std::cout << " DONE" << std::endl;
    return res;
}

template <typename Type>
static Json::Value queue_memory_test(const size_t step)
{
    std::cout << "queue-memory-test<" << typeid(Type).name() << "> " << std::flush;
    Json::Value res = get_json_template();

    for (int i = 0; i < 10; ++i) {
        size_t n = static_cast<size_t>(i + 1) * step;
        res["rows"][i]["c"][0]["v"] = std::to_string(n);

        res["cols"][1]["label"] = "(NR)StdVector";
        res["cols"][2]["label"] = "(NR)StdDeque";
        res["cols"][3]["label"] = "(NR)StdList";
        res["rows"][i]["c"][4]["v"] = queue_memory<QueueNaive, Type>(n);         std::cout << "." << std::flush;
        res["rows"][i]["c"][5]["v"] = queue_memory<QueueReclaiming, Type>(n);    std::cout << "." << std::flush;
        res["rows"][i]["c"][6]["v"] = queue_memory<QueueConservative, Type>(n);  std::cout << "." << std::flush;
        res["rows"][i]["c"][7]["v"] = queue_memory<DequeNaive, Type>(n);         std::cout << "." << std::flush;
        res["rows"][i]["c"][8]["v"] = queue_memory<DequeReclaiming, Type>(n);    std::cout << "." << std::flush;
        res["rows"][i]["c"][9]["v"] = queue_memory<DequeConservative, Type>(n);  std::cout << "." << std::flush;
    }

    std::cout << " DONE" << std::endl;
    return res;
}

template <typename Type>
static Json::Value zigzag_test(const size_t step)
{
    std::cout << "zigzag-test<" << typeid(Type).name() << "> " << std::flush;
    Json::Value res = get_json_template();

    for (int i = 0; i < 10; ++i) {
        size_t n = static_cast<size_t>(i + 1) * step;
        res["rows"][i]["c"][0]["v"] = std::to_string(n);

        get_average(zigzag<StdDeque, Type>, n); // warm-up

        res["cols"][1]["label"] = "(NR)StdVector";
        res["rows"][i]["c"][2]["v"] = get_average(zigzag<StdDeque, Type>, n);           std::cout << "." << std::flush;
        res["rows"][i]["c"][3]["v"] = get_average(zigzag<StdList, Type>, n);            std::cout << "." << std::flush;
        res["cols"][4]["label"] = "(NR)QueueNaive";
        res["cols"][5]["label"] = "(NR)QueueReclaiming";
        res["cols"][6]["label"] = "(NR)QueueConservative";
        res["rows"][i]["c"][7]["v"] = get_average(zigzag<DequeNaive, Type>, n);         std::cout << "." << std::flush;
        res["rows"][i]["c"][8]["v"] = get_average(zigzag<DequeReclaiming, Type>, n);    std::cout << "." << std::flush;
        res["rows"][i]["c"][9]["v"] = get_average(zigzag<DequeConservative, Type>, n);  std::cout << "." << std::flush;
    }

    std::cout << " DONE" << std::endl;
    return res;
}

template <typename Type>
static Json::Value zigzag_memory_test(const size_t step)
{
    std::cout << "zigzag-memory-test<" << typeid(Type).name() << "> " << std::flush;
    Json::Value res = get_json_template();

    for (int i = 0; i < 10; ++i) {
        size_t n = static_cast<size_t>(i + 1) * step;
        res["rows"][i]["c"][0]["v"] = std::to_string(n);

        res["cols"][1]["label"] = "(NR)StdVector";
        res["cols"][2]["label"] = "(NR)StdDeque";
        res["cols"][3]["label"] = "(NR)StdList";
        res["cols"][4]["label"] = "(NR)QueueNaive";
        res["cols"][5]["label"] = "(NR)QueueReclaiming";
        res["cols"][6]["label"] = "(NR)QueueConservative";
        res["rows"][i]["c"][7]["v"] = zigzag_memory<DequeNaive, Type>(n);         std::cout << "." << std::flush;
        res["rows"][i]["c"][8]["v"] = zigzag_memory<DequeReclaiming, Type>(n);    std::cout << "." << std::flush;
        res["rows"][i]["c"][9]["v"] = zigzag_memory<DequeConservative, Type>(n);  std::cout << "." << std::flush;
    }

    std::cout << " DONE" << std::endl;
    return res;
}

template <typename Type>
static Json::Value traverse_test(const size_t step)
{
    std::cout << "traverse-test<" << typeid(Type).name() << "> " << std::flush;
    Json::Value res = get_json_template();

    for (int i = 0; i < 10; ++i) {
        size_t n = static_cast<size_t>(i + 1) * step;
        res["rows"][i]["c"][0]["v"] = std::to_string(n);

        get_average(traverse<StdDeque, Type>, n); // warm-up

        res["rows"][i]["c"][1]["v"] = get_average(traverse<StdVector, Type>, n);          std::cout << "." << std::flush;
        res["rows"][i]["c"][2]["v"] = get_average(traverse<StdDeque, Type>, n);           std::cout << "." << std::flush;
        res["cols"][3]["label"] = "(NR)StdList";
        res["rows"][i]["c"][4]["v"] = get_average(traverse<QueueNaive, Type>, n);         std::cout << "." << std::flush;
        res["rows"][i]["c"][5]["v"] = get_average(traverse<QueueReclaiming, Type>, n);    std::cout << "." << std::flush;
        res["rows"][i]["c"][6]["v"] = get_average(traverse<QueueConservative, Type>, n);  std::cout << "." << std::flush;
        res["rows"][i]["c"][7]["v"] = get_average(traverse<DequeNaive, Type>, n);         std::cout << "." << std::flush;
        res["rows"][i]["c"][8]["v"] = get_average(traverse<DequeReclaiming, Type>, n);    std::cout << "." << std::flush;
        res["rows"][i]["c"][9]["v"] = get_average(traverse<DequeConservative, Type>, n);  std::cout << "." << std::flush;
    }

    std::cout << " DONE" << std::endl;
    return res;
}

template <typename Type>
static Json::Value shuffle_test(const size_t step)
{
    std::cout << "shuffle-test<" << typeid(Type).name() << "> " << std::flush;
    Json::Value res = get_json_template();

    for (int i = 0; i < 10; ++i) {
        size_t n = static_cast<size_t>(i + 1) * step;
        res["rows"][i]["c"][0]["v"] = std::to_string(n);

        get_average(shuffle<StdDeque, Type>, n); // warm-up

        res["rows"][i]["c"][1]["v"] = get_average(shuffle<StdVector, Type>, n);          std::cout << "." << std::flush;
        res["rows"][i]["c"][2]["v"] = get_average(shuffle<StdDeque, Type>, n);           std::cout << "." << std::flush;
        res["cols"][3]["label"] = "(NR)StdList";
        res["rows"][i]["c"][4]["v"] = get_average(shuffle<QueueNaive, Type>, n);         std::cout << "." << std::flush;
        res["rows"][i]["c"][5]["v"] = get_average(shuffle<QueueReclaiming, Type>, n);    std::cout << "." << std::flush;
        res["rows"][i]["c"][6]["v"] = get_average(shuffle<QueueConservative, Type>, n);  std::cout << "." << std::flush;
        res["rows"][i]["c"][7]["v"] = get_average(shuffle<DequeNaive, Type>, n);         std::cout << "." << std::flush;
        res["rows"][i]["c"][8]["v"] = get_average(shuffle<DequeReclaiming, Type>, n);    std::cout << "." << std::flush;
        res["rows"][i]["c"][9]["v"] = get_average(shuffle<DequeConservative, Type>, n);  std::cout << "." << std::flush;
    }

    std::cout << " DONE" << std::endl;
    return res;
}

template <typename Type>
static Json::Value qsort_test(const size_t step)
{
    std::cout << "qsort-test<" << typeid(Type).name() << "> " << std::flush;
    Json::Value res = get_json_template();

    for (int i = 0; i < 10; ++i) {
        size_t n = static_cast<size_t>(i + 1) * step;
        res["rows"][i]["c"][0]["v"] = std::to_string(n);

        get_average(qsort<StdDeque, Type>, n); // warm-up

        res["rows"][i]["c"][1]["v"] = get_average(qsort<StdVector, Type>, n);          std::cout << "." << std::flush;
        res["rows"][i]["c"][2]["v"] = get_average(qsort<StdDeque, Type>, n);           std::cout << "." << std::flush;
        res["cols"][3]["label"] = "(NR)StdList";
        res["rows"][i]["c"][4]["v"] = get_average(qsort<QueueNaive, Type>, n);         std::cout << "." << std::flush;
        res["rows"][i]["c"][5]["v"] = get_average(qsort<QueueReclaiming, Type>, n);    std::cout << "." << std::flush;
        res["rows"][i]["c"][6]["v"] = get_average(qsort<QueueConservative, Type>, n);  std::cout << "." << std::flush;
        res["rows"][i]["c"][7]["v"] = get_average(qsort<DequeNaive, Type>, n);         std::cout << "." << std::flush;
        res["rows"][i]["c"][8]["v"] = get_average(qsort<DequeReclaiming, Type>, n);    std::cout << "." << std::flush;
        res["rows"][i]["c"][9]["v"] = get_average(qsort<DequeConservative, Type>, n);  std::cout << "." << std::flush;
    }

    std::cout << " DONE" << std::endl;
    return res;
}

int main()
{
    std::ofstream html_file("charts.html");
    run_tests_and_print_html(html_file);
    html_file.close();
    return EXIT_SUCCESS;
}
