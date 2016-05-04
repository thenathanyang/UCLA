#include <type_traits>
#include <iostream>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <chrono>
#include <thread>
#include "Map.h"
#define CHECKTYPE(f, t) { auto p = (t)(f); (void)p; }

int main(int argc, char** argv) {
    assert(argc >= 2);
    bool checkAll = (strcmp(argv[1], "all") == 0);
    int testcase = checkAll ? 0 : atoi(argv[2]);

    switch(testcase) {
        default: break;
        case 0: {
            static_assert(std::is_default_constructible<Map>::value,
                "Map must be default-constructible.");
            static_assert(std::is_copy_constructible<Map>::value,
                "Map must be copy-constructible.");

            CHECKTYPE(&Map::operator=,      Map& (Map::*)(const Map&));
            CHECKTYPE(&Map::empty,          bool (Map::*)() const);
            CHECKTYPE(&Map::size,           int  (Map::*)() const);
            CHECKTYPE(&Map::insert,         bool (Map::*)(const KeyType&, const ValueType&));
            CHECKTYPE(&Map::update,         bool (Map::*)(const KeyType&, const ValueType&));
            CHECKTYPE(&Map::insertOrUpdate, bool (Map::*)(const KeyType&, const ValueType&));
            CHECKTYPE(&Map::erase,          bool (Map::*)(const KeyType&));
            CHECKTYPE(&Map::contains,       bool (Map::*)(const KeyType&) const);
            CHECKTYPE(&Map::get,            bool (Map::*)(const KeyType&, ValueType&) const);
            CHECKTYPE(&Map::get,            bool (Map::*)(int, KeyType&, ValueType&) const);
            CHECKTYPE(&Map::swap,           void (Map::*)(Map&));

            CHECKTYPE(combine,  bool (*)(const Map&, const Map&, Map&));
            CHECKTYPE(subtract, void (*)(const Map&, const Map&, Map&));

            if (!checkAll) break;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        case 1: {
            std::cerr << " >>> TEST EMPTY SELF-ASSIGNMENT" << std::endl;
            Map a, b;

            a = a;
            assert(a.insert("A", 10));
            assert(b.insert("B", 20));

            std::cerr << "a "; a.dump();
            std::cerr << "b "; b.dump();

            std::cerr << " >>> TEST NON-EMPTY SELF-ASSIGNMENT" << std::endl;
            Map c, d;
            assert(c.insert("A", 10));
            assert(d.insert("B", 20));

            c = c;
            std::cerr << "c "; c.dump();
            std::cerr << "d "; d.dump();
            assert(c.insert("C", 10));
            assert(d.insert("D", 20));
            std::cerr << "c "; c.dump();
            std::cerr << "d "; d.dump();

            if (!checkAll) break;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        case 2: {
            std::cerr << " >>> TEST EMPTY ASSIGNMENT" << std::endl;
            Map e;
            Map w;

            e = w;
            assert(e.insert("A", 10));
            assert(w.insert("B", 20));

            std::cerr << "e "; e.dump();
            std::cerr << "w "; w.dump();

            std::cerr << " >>> TEST NON-EMPTY ASSIGNMENT" << std::endl;
            Map f;
            assert(f.insert("A", 1));
            assert(f.insert("B", 2));
            assert(f.insert("C", 3));

            Map g;
            assert(g.insert("L", 10));

            std::cerr << "f "; f.dump();
            std::cerr << "g "; g.dump();
            std::cerr << "---" << std::endl;

            g = f;
            std::cerr << "f "; f.dump();
            std::cerr << "g "; g.dump();

            assert(f.insert("Q", 10));
            assert(g.insert("Z", 20));
            assert(g.update("B", 7));
            std::cerr << "---" << std::endl;
            std::cerr << "f "; f.dump();
            std::cerr << "g "; g.dump();

            if (!checkAll) break;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        case 3: {
            std::cerr << " >>> TEST EMPTY SWAP" << std::endl;
            Map e;
            Map w;

            e.swap(w);
            assert(e.insert("A", 10));
            assert(w.insert("B", 20));

            std::cerr << "e "; e.dump();
            std::cerr << "w "; w.dump();

            std::cerr << " >>> TEST NON-EMPTY SWAP" << std::endl;
            Map p;
            p.insert("A", 1);
            p.insert("B", 2);
            p.insert("C", 3);

            Map r;
            r.insert("D", 8);

            std::cerr << "r "; r.dump();
            std::cerr << "p "; p.dump();
            std::cerr << "..." << std::endl;

            r.swap(p);
            std::cerr << "r "; r.dump();
            std::cerr << "p "; p.dump();
            std::cerr << "..." << std::endl;

            assert(p.insert("E", 10));
            assert(r.update("C", 200));

            std::cerr << "r "; r.dump();
            std::cerr << "p "; p.dump();
            std::cerr << "---" << std::endl;

            if (!checkAll) break;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        case 4: {
            std::cerr << " >>> TEST NON-EMPTY COPY CONSTRUCTOR" << std::endl;
            Map m;

            assert(m.insert("A", 1));
            assert(m.insert("B", 2));
            assert(m.insert("C", 3));
            std::cerr << "m "; m.dump();
            std::cerr << "..." << std::endl;

            Map n(m);
            std::cerr << "n "; m.dump();
            std::cerr << "..." << std::endl;
            assert(n.update("C", 7));
            assert(n.insert("D", 8));
            std::cerr << "n "; n.dump();
            std::cerr << "m "; m.dump();
            std::cerr << "---" << std::endl;

            std::cerr << " >>> TEST R-EMPTY COPY CONSTRUCTOR" << std::endl;
            Map s;
            std::cerr << "s "; s.dump();
            std::cerr << "..." << std::endl;
            Map q(s);

            std::cerr << "q "; q.dump();
            std::cerr << "..." << std::endl;
            assert(s.size() == 0 && q.size() == 0);
            assert(s.insert("T", 9) && s.size() == 1 && q.size() == 0);
            assert(q.insert("Q", 4) && s.size() == 1 && q.size() == 1);
            std::cerr << "s "; s.dump();
            std::cerr << "q "; q.dump();
            std::cerr << "---" << std::endl;

            if (!checkAll) break;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        case 5: {
            std::cerr << "TEST MEMBER FUNCTIONS" << std::endl;
            Map m;
            KeyType k;
            ValueType v;
            KeyType ks[] = {"A","B","C"};
            ValueType vs[] = {10,20,30};

            assert(m.empty() && m.size() == 0); // map should initalize as empty
            assert(!m.update("non-existent-key", 10));  // cannot update non-existent-key
            assert(!m.contains("non-existent-key")); // does not find non-existent-key
            assert(!m.erase("non-existent-key"));   // cannot erase non-existent-key
            assert(m.insert("T", 12) && m.size() == 1 && m.get("T", v) && v == 12); // test insert elem 1
            assert(m.erase("T") && m.size() == 0 && !m.get("T", v) && v == 12); // test erase head
            assert(m.insert("T", 10) && m.insert("R", 12) && m.size() == 2); // insert 2 elements (size==2)
            assert(m.erase("T") && m.erase("R") && m.size() == 0); // test erase head w/ element after, and erase that too
            assert(m.insertOrUpdate("AA", 15) && m.size() == 1);    // insert or update always true, check for insertion
            assert(m.insert("A", 10) && m.size() == 2 && m.get("A", v) && v == 10 && m.get("AA", v) && v == 15);    // check vals
            assert(m.insert("B", 20) && m.size() == 3 && m.get("B", v) && v == 20); // check insert
            assert(m.insert("C", 4) && m.insert("D", 5) && m.insert("E", 6) && m.size() == 6); // insert elements
            assert(m.erase("C") && m.erase("D") && m.erase("E") && m.size() == 3);  // test consecutive erase from middle to end
            assert(m.insertOrUpdate("B", 30) && m.size() == 3 && m.get("B", v) && v == 30); //check update
            assert(m.erase("B") && m.size() == 2 && !m.get("B", v) && v == 30); // check erase end, check erased
            assert(m.contains("A") && m.contains("AA") && !m.contains("B")); // check erased
            assert(!m.erase("B"));  // cannot erase erased elem (check that its actually out of list)
            assert(m.size() == 2);  // assert size
            assert(m.insert("B", 20) && m.insert("C", 30) && m.erase("AA")); // insert two elements, check for erase at beginning
            assert(m.size() == 3);
            assert(m.get("A", v) && v == 10);
            for (int i = 0; i < m.size(); i++)
                assert(m.get(i, k, v) && k == ks[i] && v == vs[i]);

            m.dump();

            if (!checkAll) break;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        case 6: {
            std::cerr << "TEST COMBINE MAPS" << std::endl;
        {
            Map m, n, o, p;
            KeyType k; ValueType v;

            m.insert("A", 10);
            m.insert("B", 20);
            m.insert("C", 30);
            m.insert("D", 40);

            n.insert("E", 50);
            n.insert("B", 20);
            n.insert("C", 40);

            assert(!combine(m, n, o) && o.size() == 4);
            assert(o.contains("A") && o.contains("B") && o.contains("D") && o.contains("E"));
            assert(o.get("B",v) && v == 20 && !o.get("C", v) && v == 20);
            std::cerr << "m "; m.dump();
            std::cerr << "n "; n.dump();
            std::cerr << "o "; o.dump();
            std::cerr << "---" << std::endl;
        }

            std::cerr << "TEST COMBINE SAMEREF MAPS" << std::endl;
        {
            Map m, n, o, p;
            KeyType k; ValueType v;

            m.insert("A", 10);
            m.insert("B", 20);
            m.insert("C", 30);
            m.insert("D", 40);

            assert(combine(m, m, n) && m.size() == n.size() && &m != &n);
            std::cerr << "m "; m.dump();
            std::cerr << "n "; n.dump();
            std::cerr << "..." << std::endl;
            assert(n.erase("D") && n.size() == 3 && m.size() == 4);
            std::cerr << "m "; m.dump();
            std::cerr << "n "; n.dump();
            std::cerr << "---" << std::endl;
        }

            std::cerr << "TEST COMBINE MAPS (m,n) -> m" << std::endl;
        {
            Map m, n, o, p;
            KeyType k; ValueType v;

            m.insert("A", 10);
            m.insert("B", 20);
            m.insert("C", 30);
            m.insert("D", 40);

            n.insert("E", 50);
            n.insert("B", 20);
            n.insert("C", 40);

            std::cerr << "m "; m.dump();
            std::cerr << "n "; n.dump();
            std::cerr << "..." << std::endl;

            assert(!combine(m, n, m) && m.size() == 4);
            assert(m.contains("A") && m.contains("B") && m.contains("D") && m.contains("E"));
            assert(m.get("B",v) && v == 20 && !m.get("C", v) && v == 20);

            std::cerr << "m "; m.dump();
            std::cerr << "n "; n.dump();
            std::cerr << "---" << std::endl;
        }

            std::cerr << "TEST COMBINE SAMEVAL MAPS" << std::endl;
        {
            Map m, n, o, p;
            KeyType k; ValueType v;

            m.insert("A", 10);
            m.insert("B", 20);
            m.insert("C", 30);
            m.insert("D", 40);

            n.insert("A", 10);
            n.insert("B", 20);
            n.insert("C", 30);
            n.insert("D", 40);

            std::cerr << "m "; m.dump();
            std::cerr << "n "; n.dump();
            std::cerr << "..." << std::endl;

            assert(combine(m, n, o) && o.size() == 4);
            assert(o.contains("A") && o.contains("B") && o.contains("C") && o.contains("D"));
            assert(o.get("B",v) && v == 20 && o.get("C", v) && v == 30);

            std::cerr << "m "; m.dump();
            std::cerr << "n "; n.dump();
            std::cerr << "o "; o.dump();
            std::cerr << "---" << std::endl;
        }

            std::cerr << "TEST COMBINE 1 EMPTY MAP" << std::endl;
        {
            Map m, n, o, p;
            KeyType k; ValueType v;

            m.insert("A", 10);
            m.insert("B", 20);
            m.insert("C", 30);
            m.insert("D", 40);

            std::cerr << "m "; m.dump();
            std::cerr << "n "; n.dump();
            std::cerr << "..." << std::endl;

            assert(combine(m, n, o) && o.size() == 4);
            assert(o.contains("A") && o.contains("B") && o.contains("C") && o.contains("D"));
            assert(o.get("B",v) && v == 20 && o.get("C", v) && v == 30);

            std::cerr << "m "; m.dump();
            std::cerr << "n "; n.dump();
            std::cerr << "o "; o.dump();
            std::cerr << "---" << std::endl;
        }

            if (!checkAll) break;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        case 7: {
            // TEST SUBTRACT
            std::cerr << "TEST SUBTRACT" << std::endl;
            {
                Map m, n, o, p;
                KeyType k; ValueType v;

                m.insert("A", 10);
                m.insert("B", 20);
                m.insert("C", 30);
                m.insert("D", 40);

                n.insert("E", 50);
                n.insert("B", 20);
                n.insert("C", 40);

                subtract(m, n, o);
                assert(o.size() == 2);
                assert(o.contains("A") && o.contains("D"));
                assert(o.get("A",v) && v == 10 && !o.get("C", v) && v == 10);
                o.dump();
                std::cerr << "---" << std::endl;
            }

            std::cerr << "TEST SELFREF SUBTRACT" << std::endl;
            {
                Map m, n, o, p;
                KeyType k; ValueType v;

                m.insert("A", 10);
                m.insert("B", 20);
                m.insert("C", 30);
                m.insert("D", 40);

                subtract(m, m, n);
                assert(&m != &n && n.size() == 0 && m.size() == 4);
                assert(n.insert("D", 40) && n.size() == 1 && m.size() == 4);
                std::cerr << "n "; n.dump();
                std::cerr << "m "; m.dump();
                std::cerr << "---" << std::endl;
            }

            std::cerr << "TEST SUBTRACT (m, n) -> m" << std::endl;
            {
                Map m, n, o, p;
                KeyType k; ValueType v;

                m.insert("A", 10);
                m.insert("B", 20);
                m.insert("C", 30);
                m.insert("D", 40);

                n.insert("E", 50);
                n.insert("B", 20);
                n.insert("C", 40);

                std::cerr << "m "; m.dump();
                std::cerr << "n "; n.dump();
                std::cerr << "---" << std::endl;

                subtract(m, n, m);
                assert(n.size() == 3 && m.size() == 2);
                assert(m.contains("A") && m.contains("D"));
                assert(m.get("A",v) && v == 10 && !m.get("C", v) && v == 10);

                std::cerr << "m "; m.dump();
                std::cerr << "n "; n.dump();
                std::cerr << "..." << std::endl;

                assert(m.insert("F", 50) && m.size() == 3 && n.size() == 3);
                assert(n.insert("G", 60) && m.size() == 3 && n.size() == 4);
                std::cerr << "m "; m.dump();
                std::cerr << "n "; n.dump();
                std::cerr << "---" << std::endl;
            }

            std::cerr << "TEST EMPTY SUBTRACT" << std::endl;
            {
                Map n, m, o, p;
                assert(m.insert("A", 10));
                assert(m.insert("B", 20));
                assert(m.insert("C", 30));

                subtract(n, m, o);
                assert(o.size() == 0);
                subtract(m, n, o);
                assert(o.size() == 3);
                subtract(m, n, m);
                assert(m.size() == 3);
                subtract(m, n, n);
                assert(n.size() == 3);
                subtract(p, m, m);
                assert(m.size() == 0);
            }

            if (!checkAll) break;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        case 8: {
            std::cerr << "TEST SMALLBERG TEST 1" <<std::endl;
            Map m;
            assert(m.insert("Fred", 123));
            assert(m.insert("Ethel", 456));
            assert(m.size() == 2);
            double d = 42;
            assert(m.get("Fred", d)  &&  d == 123);
            d = 42;
            std::string s1;
            assert(m.get(0, s1, d)  &&
                   ((s1 == "Fred"  &&  d == 123)  ||  (s1 == "Ethel"  &&  d == 456)));
            std::string s2;
            assert(m.get(1, s2, d)  &&  s1 != s2  &&
                   ((s2 == "Fred"  &&  d == 123)  ||  (s2 == "Ethel"  &&  d == 456)));

            if (!checkAll) break;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        case 9: {
            std::cerr << "MERP TEST" << std::endl;
            Map e;
            assert(e.insert("Merp", 10));
            e.dump();
            assert(e.size() == 1);
            assert(e.erase("Merp"));
            e.dump();
            assert(e.empty());

            if (!checkAll) break;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

    }

    std::cerr << "ALL TESTS PASSED" << std::endl;
    return 0;
}
