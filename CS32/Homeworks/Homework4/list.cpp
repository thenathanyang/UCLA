void listAll(const Class* c, string path)  // two-parameter overload
{
    if (c == nullptr) return;

    // cout << path << c->name() << endl;
    path += c->name();
    cout << path << endl;               // Path includes the classes that come before it

    for (vector<Class*>::const_iterator it = c->subclasses().begin(); it != c->subclasses().end(); it++)
        listAll((*it), path + "=>");   
}
