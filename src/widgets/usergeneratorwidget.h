#ifndef USERGENERATORWIDGET_H
#define USERGENERATORWIDGET_H

#include "customwidget.h"
#include <QWidget>
#include <qboxlayout.h>
#include <qspinbox.h>

namespace Ui {
class UserGeneratorWidget;
}

class UserGeneratorWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit UserGeneratorWidget(QWidget *parent = nullptr);
    ~UserGeneratorWidget();
    bool canSaveFiles() const override;
    bool canBasicEdit() const override;
    void save() override;
    void saveAs() override;

private:
    Ui::UserGeneratorWidget *ui;
    QString openedFile;

private:
    QString getRandomQString(const QStringList& list) const;

private slots:
    void generate();

private:
    const QStringList firstNames {
        "Olivia", "Emma", "Charlotte", "Amelia", "Sophia", "Mia", "Isabella", "Ava", "Evelyn", "Luna", "Harper", "Camila", "Gianna", "Elizabeth", "Ellie", "Sofia", "Ella", "Penelope", "Nova",
        "Aurora", "Scarlett", "Mila", "Chloe", "Layla", "Zoe", "Nora", "Lily", "Iris", "Paisley", "Violet", "Willow", "Grace", "Stella", "Naomi", "Savannah", "Aria", "Everly", "Lillian", "Madison",
        "Natalie", "Hazel", "Brooklyn", "Aaliyah", "Audrey", "Hannah", "Zoey", "Alice", "Elena", "Maya", "Victoria", "Liam", "Noah", "Oliver", "James", "Elijah", "William", "Henry", "Lucas",
        "Benjamin", "Theodore", "Mateo", "Levi", "Sebastian", "Jack", "Ethan", "Mason", "Luke", "Ezra", "Asher", "Grayson", "Leo", "Samuel", "Daniel", "Aiden", "Joseph", "Owen", "Wyatt", "Caleb",
        "Hudson", "Lincoln", "Jayden", "Jackson", "Julian", "Gabriel", "Alexander", "Elias", "Maverick", "Michael", "Isaiah", "Nicholas", "David", "Miles", "Jeremiah", "John", "Nathan", "Adrian",
        "Angel", "Christopher", "Jaxon", "Jonathan"
    };
    const QStringList lastNames {
        "Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia", "Miller", "Davis", "Rodriguez", "Martinez", "Hernandez", "Lopez", "Gonzalez", "Wilson", "Anderson", "Thomas", "Taylor", "Moore",
        "Jackson", "Martin", "Lee", "Perez", "Thompson", "White", "Harris", "Sanchez", "Clark", "Ramirez", "Lewis", "Robinson", "Walker", "Hall", "Young", "Allen", "King", "Wright", "Scott", "Torres",
        "Nguyen", "Hill", "Flores", "Green", "Adams", "Nelson", "Baker", "Hall", "Rivera", "Campbell", "Mitchell", "Carter", "Roberts", "Gomez", "Phillips", "Evans", "Turner", "Diaz", "Parker", "Cruz",
        "Edwards", "Collins", "Reyes", "Stewart", "Morris", "Morales", "Murphy", "Cook", "Rogers", "Gupta", "Morgan", "Peterson", "Cooper", "Reed", "Bailey", "Bell", "Coleman", "Howard", "Ward", "Foster",
        "Gonzales", "Watson", "Bryant", "Russell", "Patterson", "Gray", "Hughes", "James", "Washington", "Simmons", "Bennett", "Richardson", "Cox", "Alexander", "Wood", "Coleman", "Kelly", "Henderson",
        "Chavez", "Diaz"
    };

};

#endif // USERGENERATORWIDGET_H
