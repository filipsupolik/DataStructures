#pragma once
class Algorithm {
public:
	template<typename Iterator, typename Predicate, typename ResultList, typename Inserter>
	static void findAndInsertElement(Iterator zaciatok, Iterator koniec, Predicate predikat, ResultList& result, Inserter insert) {
		while (zaciatok != koniec) {
			if (predikat(*zaciatok)) {
				insert(result, *zaciatok);
			}
			++zaciatok;
		}
	}
};