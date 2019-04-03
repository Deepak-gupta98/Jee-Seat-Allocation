//Gale–Shapley algorithm

class EligibleStudent
{
  public:
    string rollNo;
    int AIR;
    string category;
    bool isAlloted;
    int AllotedCode;
    int AllotedPreference;
    vector<int> preference;

    EligibleStudent()
    {
        isAlloted = false;
    }

    EligibleStudent(string roll, string air, string cat, vector<int> pref) : preference(pref)
    {
        rollNo = rollNo;
        AIR = stoi(air);
        category = cat;
        isAlloted = false;
        AllotedPreference = INT_MAX;
    }
};

class AvailableProgram
{
  public:
    int Totalcapacity;
    int GenCap;
    int ObcCap;
    int scCap;
    int programID;
    bool Engaged;
    EligibleStudent allotedStudent;

    AvailableProgram()
    {
        Engaged = false;
    }

    AvailableProgram(string proID, int cap)
    {
        Totalcapacity = cap;
        programID = stoi(proID);
        GenCap = Totalcapacity / 2;
        ObcCap = Totalcapacity / 4;
        scCap = Totalcapacity / 4;
    }
};

map<int, EligibleStudent> EligibleStudents;
map<int, AvailableProgram> AvailablePrograms;

void ReadStudents()
{
    ifstream in;
    Student student;
    string StudentfileName = "Records/Student.dat";
    in.open(StudentfileName.c_str());
    while (in.read((char *)&student, sizeof(student)))
    {
        EligibleStudent newStudent(student.roll, student.air, student.category, student.preference);
        EligibleStudents[newStudent.AIR] = newStudent;
    }
    in.close();
}

void ReadPrograms()
{
    ifstream in;
    Program program;
    string fileName = "Records/Program.dat";
    in.open(fileName.c_str());
    while (in.read((char *)&program, sizeof(program)))
    {
        AvailableProgram newProgram(program.program_code, program.Capacity);
        AvailablePrograms[newProgram.programID] = newProgram;
    }
    in.close();
}

//EligibleStudents is already sorted
//Gen+OBC+SC/ST
void FirstPass()
{
    for (auto i : EligibleStudents)
    {
        for (int j = 0; j < i.second.preference.size(); j++)
        {
            if (AvailablePrograms[i.second.preference[j]].GenCap > 0)
            {
                AvailablePrograms[i.second.preference[j]].GenCap--;
                i.second.isAlloted = true;
                i.second.AllotedCode = i.second.preference[j];
                i.second.AllotedPreference = j;
                break;
            }
        }
    }
}

void SecondPass()
{
    for (auto i : EligibleStudents)
    {
        if (i.second.category.compare("OBC") == 0)
        {
            for (int j = 0; j < i.second.preference.size(); j++)
            {
                if (AvailablePrograms[i.second.preference[j]].ObcCap > 0 && i.second.AllotedPreference < j)
                {
                    AvailablePrograms[i.second.preference[j]].ObcCap--;

                    if (i.second.isAlloted)
                    {
                        AvailablePrograms[i.second.AllotedCode].GenCap++;
                    }

                    i.second.isAlloted = true;
                    i.second.AllotedCode = i.second.preference[j];
                    i.second.AllotedPreference = j;
                    break;
                }
            }
        }
        else if (i.second.category.compare("SC/ST") == 0)
        {
            for (int j = 0; j < i.second.preference.size(); j++)
            {
                if (AvailablePrograms[i.second.preference[j]].scCap > 0 && i.second.AllotedPreference < j)
                {
                    AvailablePrograms[i.second.preference[j]].scCap--;

                    if (i.second.isAlloted)
                    {
                        AvailablePrograms[i.second.AllotedCode].GenCap++;
                    }

                    i.second.isAlloted = true;
                    i.second.AllotedCode = i.second.preference[j];
                    i.second.AllotedPreference = j;
                    break;
                }
            }
        }
    }
}

void ThirdPass()
{
    for (auto i : EligibleStudents)
    {
        for (int j = 0; j < i.second.preference.size(); j++)
        {
            if (AvailablePrograms[i.second.preference[j]].GenCap > 0 && i.second.AllotedPreference < j)
            {
                if (i.second.isAlloted)
                {
                    if (i.second.category.compare("GEN") == 0)
                    {
                        AvailablePrograms[i.second.AllotedCode].GenCap++;
                    }
                    else if (i.second.category.compare("OBC") == 0)
                    {
                        AvailablePrograms[i.second.AllotedCode].ObcCap++;
                    }
                    else
                    {
                        AvailablePrograms[i.second.AllotedCode].scCap++;
                    }
                }
                AvailablePrograms[i.second.preference[j]].GenCap--;
                i.second.isAlloted = true;
                i.second.AllotedCode = i.second.preference[j];
                i.second.AllotedPreference = j;
                break;
            }
        }
    }
}

//Eligible students will make proposals
void Allocate()
{
    ReadStudents();
    // ReadPrograms();
    // FirstPass();
    // SecondPass();
    // ThirdPass();
}