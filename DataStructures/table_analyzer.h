#pragma once
#include <complexities/complexity_analyzer.h>
#include <libds/adt/table.h>
#include <random>
#include <vector>

namespace ds::utils
{
    template<class Table>
    class TableAnalyzer : public ComplexityAnalyzer<Table>
    {
    protected:
        explicit TableAnalyzer(const std::string& name);

    protected:
        void growToSize(Table& structure, size_t size) override;
    };
    //===================================================
    template<class Table>
    class TableInsertAnalyzer : public TableAnalyzer<Table>
    {
    public:
        explicit TableInsertAnalyzer(const std::string& name);

    protected:
        void executeOperation(Table& structure) override;
    };

    class TablesAnalyzer: public CompositeAnalyzer
    {
    public:
        TablesAnalyzer();
    };

    //===============================================================
    template<class Table>
    inline TableAnalyzer<Table>::TableAnalyzer(const std::string& name):
        ComplexityAnalyzer<Table>(name)
    {
    }
    template<class Table>
    inline void TableAnalyzer<Table>::growToSize(Table& structure, size_t size)
    {
    }
    template<class Table>
    inline TableInsertAnalyzer<Table>::TableInsertAnalyzer(const std::string& name):
        TableAnalyzer<Table>(name)
    {
    }
    template<class Table>
    inline void TableInsertAnalyzer<Table>::executeOperation(Table& structure)
    {
    }

    inline TablesAnalyzer::TablesAnalyzer() :
        CompositeAnalyzer("Tables")
    {
        this->addAnalyzer(std::make_unique<TableInsertAnalyzer<ds::adt::SortedSequenceTable<int, int>>("table-insert"));
    }
}