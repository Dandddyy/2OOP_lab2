#include <iostream>
#include <vector>
using namespace std;

class Task {
public:
    virtual ~Task() {}
    virtual string getTitle() = 0;
    virtual string getDescription() = 0;
    virtual string getStartDate() = 0;
    virtual string getEndDate() = 0;
    virtual int getPriority() = 0;
    virtual bool IsComposite() const {
        return false;
    }
};

class SimpleTask : public Task {
public:
    SimpleTask(string title, string description, string startDate,
        string endDate, int priority) : title_(title), description_(description),
        startDate_(startDate), endDate_(endDate), priority_(priority) {}

    string getTitle() override { return title_; }
    string getDescription() override { return description_; }
    string getStartDate() override { return startDate_; }
    string getEndDate() override { return endDate_; }
    int getPriority() override { return priority_; }

private:
    string title_;
    string description_;
    string startDate_;
    string endDate_;
    int priority_;
};

class TaskFactory {
public:
    virtual ~TaskFactory() {}
    virtual Task* CreateTask(string title, string description,
        string startDate, string endDate, int priority) = 0;
};

class SimpleTaskFactory : public TaskFactory {
public:
    Task* CreateTask(string title, string description,
        string startDate, string endDate, int priority) override {
        return new SimpleTask(title, description, startDate, endDate, priority);
    }
};

class CompositeTask : public Task {
public:
    CompositeTask(string title, string description, string startDate,
        string endDate, int priority) : title_(title), description_(description),
        startDate_(startDate), endDate_(endDate), priority_(priority) {}

    ~CompositeTask() {
        for (Task* task : tasks_) {
            delete task;
        }
    }

    void Add(Task* task) {
        tasks_.push_back(task);
    }

    string getTitle() override { return title_; }
    string getDescription() override { return description_; }
    string getStartDate() override { return startDate_; }
    string getEndDate() override { return endDate_; }
    int getPriority() override { return priority_; }
    bool IsComposite() const override {
        return true;
    }

    vector<Task*> getTasks() { return tasks_; }

private:
    string title_;
    string description_;
    string startDate_;
    string endDate_;
    int priority_;
    vector<Task*> tasks_;
};

class Iterator {
public:
    virtual ~Iterator() {}
    virtual bool HasNext() = 0;
    virtual Task* Next() = 0;
};

class TaskIterator : public Iterator {
public:
    TaskIterator(vector<Task*> tasks) : tasks_(tasks) {
        current_ = 0;
    }

    bool HasNext() override {
        return current_ < tasks_.size();
    }

    Task* Next() override {
        Task* task = tasks_[current_];
        current_++;
        return task;
    }

private:
    vector<Task*> tasks_;
    int current_;
};

void PrintTasks(const vector<Task*>& tasks) {
    Iterator* iterator = new TaskIterator(tasks);

    while (iterator->HasNext()) {
        Task* task = iterator->Next();
        cout << "Title: " << task->getTitle() << endl;
        cout << "Description: " << task->getDescription() << endl;
        cout << "Start date: " << task->getStartDate() << endl;
        cout << "End date: " << task->getEndDate() << endl;
        cout << "Priority: " << task->getPriority() << endl;

        if (task->IsComposite()) {
            CompositeTask* compositeTask = dynamic_cast<CompositeTask*>(task);
            vector<Task*> subTasks = compositeTask->getTasks();

            cout << "Subtasks: " << endl;
            for (auto subTask : subTasks) {
                cout << "\tTitle: " << subTask->getTitle() << endl;
                cout << "\tDescription: " << subTask->getDescription() << endl;
                cout << "\tStart date: " << subTask->getStartDate() << endl;
                cout << "\tEnd date: " << subTask->getEndDate() << endl;
                cout << "\tPriority: " << subTask->getPriority() << endl;
                cout << endl;
            }
        }

        cout << endl;
    }
    delete iterator;
}

int main() {
    TaskFactory* factory = new SimpleTaskFactory();

    Task* task1 = factory->CreateTask("Do laundry", "Wash clothes", "2023-05-01",
        "2023-05-02", 2);

    CompositeTask* task2 = new CompositeTask("Weekend chores", "Clean house and run errands",
        "2023-05-06", "2023-05-07", 1);
    Task* task2_1 = factory->CreateTask("Clean bathroom", "Scrub toilet and sink",
        "2023-05-06", "2023-05-06", 2);
    Task* task2_2 = factory->CreateTask("Grocery shopping", "Buy groceries for the week",
        "2023-05-07", "2023-05-07", 1);
    task2->Add(task2_1);
    task2->Add(task2_2);

    Task* task3 = factory->CreateTask("Finish project", "Complete the final report",
        "2023-05-10", "2023-05-20", 3);

    vector<Task*> tasks;
    tasks.push_back(task1);
    tasks.push_back(task2);
    tasks.push_back(task3);

    PrintTasks(tasks);

    delete task1;
    delete task2;
    delete task3;
    delete factory;

    return 0;
}
