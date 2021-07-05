/********************/
/*  By Left Studio  */
/*     @Ho 229      */
/********************/

#ifndef POETRY_H
#define POETRY_H

#include <QDebug>
#include <QString>

/**
 * @brief Class Poetry
 */
class Poetry
{

public:
    QString title;
    QString poetry;

    // Optional
    QString dynasty;
    QString author;
    QString notes;
    QString translate;
    QString description;

    QString pid;        // Baidu Wenxue PID

public:
    bool isEmpty() const { return title.isEmpty() || poetry.isEmpty(); }

    friend QDebug operator<<(QDebug dbg, const Poetry &poem)
    {
        if(poem.isEmpty())
            dbg.nospace() << "Poetry(" << poem << ") is empty.";
        else
        {
            dbg.nospace() << "\ntitle: " << poem.title
                          << "\ndynasty: " << poem.dynasty << "  author: " << poem.author
                          << "\npoetry: " << poem.poetry
                          << "\nnotes: " << poem.notes
                          << "\ntranslate: " << poem.translate
                          << "\ndescription: " << poem.description;
        }

        return dbg.maybeSpace();
    }
};

Q_DECLARE_METATYPE(Poetry)

#endif // POETRY_H
