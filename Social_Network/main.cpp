#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <limits>
#include <ctime>
#include <algorithm> 
using namespace std;

class Post {
public:
    int likes;
    string author;
    time_t timestamp;
    string content;
    vector<string> comments;

    Post(const string &content, const string& author)
        : content(content), author(author), timestamp(time(0)), likes(0) {}

    void displayPost() const {
        cout << "Post by " << author << " at " << ctime(&timestamp);
        cout << content << endl;
        cout << "Likes: " << likes << " Comments: " << comments.size() << endl;
        cout << "--------------------------------------" << endl;
    }
    void displayComments()
    {
        if(comments.size()> 0)
        {
            int idx = 1;
            for(const auto &comment: comments)
            {
                cout<<idx<<". "<<comment<<endl;
                idx++;
            }
        }
    }
};

class User {
public:
    string username;
    string name;
    string location;
    int age;
    unordered_set<string> friends;
    unordered_set<string> pendingRequests;
    vector<Post> post;

    User() : age(0) {}
    User(const string& uname, const string& n, const string& loc, int a)
        : username(uname), name(n), location(loc), age(a) {}

    void sendFriendRequest(User& recipient);
    void acceptFriendRequest(User& sender);
    void rejectFriendRequest(User& sender);
    void displayFriends();
    void displayPendingRequests();
    void setName(const string& newName);
    void setLocation(const string& newLoc);
    void setAge(int newAge);
    void createPost(const string &content);
    void likePost(Post &post);
    void commentOnPost(Post& post, const string &comment);
    void viewOthersPost();
    void viewSelfPost();
};

void User::createPost(const string &content) {
    post.emplace_back(content, username);
    cout << "Post Created by: " << username << endl;
}

void User::likePost(Post &post) {
    post.likes++;
    cout << username << " liked the post by " << post.author << endl;
}

void User::commentOnPost(Post &post, const string& comment) {
    post.comments.push_back(username + ": " + comment);
    cout << username << " commented on the post by " << post.author << endl;
}

void User::sendFriendRequest(User& recipient) {
    if (friends.find(username) == friends.end() && pendingRequests.find(username) == friends.end()) {
        recipient.pendingRequests.insert(username);
        cout << "Sent Friend Request To: " << recipient.username << endl;
    } else {
        cout << "You are either already friends or you already have a pending request from: " << recipient.username << endl;
    }
}

void User::acceptFriendRequest(User& sender) {
    if (pendingRequests.find(sender.username) != pendingRequests.end()) {
        pendingRequests.erase(sender.username);
        friends.insert(sender.username);
        sender.friends.insert(username);
        cout << "Friend Request Accepted from: " << sender.username << endl;
    } else {
        cout << "No Pending Request From: " << sender.username << endl;
    }
}

void User::rejectFriendRequest(User& sender) {
    if (pendingRequests.find(sender.username) != pendingRequests.end()) {
        pendingRequests.erase(sender.username);
        cout << "Friend Request from: " << sender.username << " rejected!!" << endl;
    } else {
        cout << "No Pending Request From: " << sender.username << endl;
    }
}

void User::displayFriends() {
    if (!friends.empty()) {
        int idx = 1;
        cout << "*******************Friends********************" << endl;
        for (const auto& friendUsername : friends) {
            cout << idx << ". " << friendUsername << endl;
            idx++;
        }
    } else {
        cout << "You don't have any friends!" << endl;
    }
}

void User::displayPendingRequests() {
    if (!pendingRequests.empty()) {
        int idx = 1;
        cout << "*******************Pending Friend Requests********************" << endl;
        for (const auto& pendingUsername : pendingRequests) {
            cout << idx << ". " << pendingUsername << endl;
            idx++;
        }
    } else {
        cout << "You don't have any pending friend requests!" << endl;
    }
}

void User::setName(const string& newName) {
    string oldName = name;
    name = newName;
    cout << "Updated Name from: " << oldName << " to: " << newName << endl;
}

void User::setLocation(const string& newLoc) {
    string oldLoc = location;
    location = newLoc;
    cout << "Updated Location from: " << oldLoc << " to: " << newLoc << endl;
}

void User::setAge(int newAge) {
    if (newAge > 100) {
        cout << "Invalid Age Value!!!" << endl;
        return;
    }
    int oldAge = age;
    age = newAge;
    cout << "Updated age from: " << oldAge << " to: " << newAge << endl;
}

class SocialNetwork {
public:
    unordered_map<string, User> users;
    void addUser(const string& username, const string& name, const string& location, int age);
    User* findUser(const string& username);
    void displayAllPost();
    vector<string> recommendFriends(const string& username);
    vector<string> searchUsers(const string &searchTerm);
};

void SocialNetwork::displayAllPost() {
    for (const auto &userPair : users) {
        const User &user = userPair.second;
        for (const auto &post : user.post) {
            post.displayPost();
        }
    }
}

void SocialNetwork::addUser(const string& username, const string& name, const string& location, int age) {
    if (users.find(username) == users.end()) {
        users[username] = User(username, name, location, age);
        cout << "Username: " << username << " added!" << endl;
    } else {
        cout << "Username Already Exists" << endl;
    }
}

User* SocialNetwork::findUser(const string& username) {
    if (users.find(username) != users.end()) {
        return &users[username];
    } else {
        return nullptr;
    }
}

vector<string> SocialNetwork::recommendFriends(const string& username) 
{
    vector<string> recommendations;
    User* user = findUser(username);
    if(!user)
    {
        cout<<"User Not Found!!"<<endl;
        return recommendations;
    }
    unordered_map<string,int>mutualFriendCount;
    
    // outer loop for user->friends
    for(const auto& userFriends : user->friends)
    {
        User* friendUser = findUser(userFriends);
        // inner loop for friends->friends
        if(friendUser)
        {
        for(const auto &mutualFriend : friendUser->friends)
        {
            if(mutualFriend != user->username && user->friends.find(mutualFriend)==user->friends.end())
            {
                mutualFriendCount[mutualFriend]++;
            }
        }
        
        }
    }
    
    for(const auto &mutualFriendPair : mutualFriendCount)
    {
        if(mutualFriendPair.second >= 1)
        {
            recommendations.push_back(mutualFriendPair.first);
        }
    }
    
    return recommendations;
}

vector<string>  SocialNetwork::searchUsers(const string &searchTerm)
{
    // 1. convert to searchterm to lower case
    string lowersearchTerm = searchTerm;
    vector<string> results;
    transform(lowersearchTerm.begin(),lowersearchTerm.end(),lowersearchTerm.begin(),::tolower);
    for(const auto &userPair : users)
    {
        const User &user = userPair.second;
        string lowerUsername = user.username;
        string lowerName = user.name;
        
        transform(lowerUsername.begin(),lowerUsername.end(),lowerUsername.begin(),::tolower);
        transform(lowerName.begin(),lowerName.end(),lowerName.begin(),::tolower);
        
        
        if(lowerUsername.find(lowersearchTerm)!=string::npos || lowerName.find(lowersearchTerm)!=string::npos)
        {
            results.push_back(user.username);
        }
    }
    
    return results;
    
}
void displayMenu() {
    cout << "\n********** Social Network Menu **********\n";
    cout << "1. Add User\n";
    cout << "2. Send Friend Request\n";
    cout << "3. Accept Friend Request\n";
    cout << "4. Reject Friend Request\n";
    cout << "5. Display Friends\n";
    cout << "6. Display Pending Requests\n";
    cout << "7. Set User Info\n";
    cout << "8. Create Post\n";
    cout << "9. Like Post\n";
    cout << "10. Comment on Post\n";
    cout << "11. Display All Posts\n";
    cout << "12. Display Comment On Post By Index\n";
    cout << "13. Recommend Friends\n";
    cout << "14. Search Users\n";
    cout << "15. Exit\n";
    cout << "*****************************************\n";
    cout << "Enter your choice: ";
}


int main() {
    SocialNetwork network;
    int choice;
    string username, name, location, friendUsername, newName, newLocation, content, comment,searchTerm;
    int age, newAge;

    while (true) {
        displayMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter Username: ";
            cin >> username;
            if (network.users.find(username) != network.users.end()) {
                cout << "Sorry This Username Is Already Taken Try Other!!" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            cout << "Enter Name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter Location: ";
            getline(cin, location);
            cout << "Enter Age: ";
            cin >> age;
            network.addUser(username, name, location, age);
            break;

        case 2:
            cout << "Enter Your Username: ";
            cin >> username;
            cout << "Enter Friend's Username: ";
            cin >> friendUsername;
            {
                User* user = network.findUser(username);
                User* friendUser = network.findUser(friendUsername);
                if (user && friendUser) {
                    user->sendFriendRequest(*friendUser);
                } else {
                    cout << "One or both users not found!" << endl;
                }
            }
            break;

        case 3:
            cout << "Enter Your Username: ";
            cin >> username;
            cout << "Enter Sender's Username: ";
            cin >> friendUsername;
            {
                User* user = network.findUser(username);
                User* sender = network.findUser(friendUsername);
                if (user && sender) {
                    user->acceptFriendRequest(*sender);
                } else {
                    cout << "One or both users not found!" << endl;
                }
            }
            break;

        case 4:
            cout << "Enter Your Username: ";
            cin >> username;
            cout << "Enter Sender's Username: ";
            cin >> friendUsername;
            {
                User* user = network.findUser(username);
                User* sender = network.findUser(friendUsername);
                if (user && sender) {
                    user->rejectFriendRequest(*sender);
                } else {
                    cout << "One or both users not found!" << endl;
                }
            }
            break;

        case 5:
            cout << "Enter Your Username: ";
            cin >> username;
            {
                User* user = network.findUser(username);
                if (user) {
                    user->displayFriends();
                } else {
                    cout << "User not found!" << endl;
                }
            }
            break;

        case 6:
            cout << "Enter Your Username: ";
            cin >> username;
            {
                User* user = network.findUser(username);
                if (user) {
                    user->displayPendingRequests();
                } else {
                    cout << "User not found!" << endl;
                }
            }
            break;

        case 7:
            cout << "Enter Your Username: ";
            cin >> username;
            {
                User* user = network.findUser(username);
                if (user) {
                    cout << "1. Change Name\n2. Change Location\n3. Change Age\n";
                    cout << "Enter your choice: ";
                    int updateChoice;
                    cin >> updateChoice;
                    switch (updateChoice) {
                    case 1:
                        cout << "Enter New Name: ";
                        cin.ignore();
                        getline(cin, newName);
                        user->setName(newName);
                        break;
                    case 2:
                        cout << "Enter New Location: ";
                        cin.ignore();
                        getline(cin, newLocation);
                        user->setLocation(newLocation);
                        break;
                    case 3:
                        cout << "Enter New Age: ";
                        cin >> newAge;
                        user->setAge(newAge);
                        break;
                    default:
                        cout << "Invalid choice!" << endl;
                    }
                } else {
                    cout << "User not found!" << endl;
                }
            }
            break;

        case 8:
            cout << "Enter Your Username: ";
            cin >> username;
            {
                User* user = network.findUser(username);
                if (user) {
                    cout << "Enter Post Content: ";
                    cin.ignore();
                    getline(cin, content);
                    user->createPost(content);
                } else {
                    cout << "User not found!" << endl;
                }
            }
            break;

        case 9:
            cout << "Enter Your Username: ";
            cin >> username;
            cout << "Enter Post Author's Username: ";
            cin >> friendUsername;
            {
                User* user = network.findUser(username);
                User* postAuthor = network.findUser(friendUsername);
                if (user && postAuthor) {
                    cout << "Enter Post Index: ";
                    int postIndex;
                    cin >> postIndex;
                    if (postIndex >= 0 && postIndex < postAuthor->post.size()) {
                        user->likePost(postAuthor->post[postIndex]);
                    } else {
                        cout << "Invalid Post Index!" << endl;
                    }
                } else {
                    cout << "One or both users not found!" << endl;
                }
            }
            break;

        case 10:
            cout << "Enter Your Username: ";
            cin >> username;
            cout << "Enter Post Author's Username: ";
            cin >> friendUsername;
            {
                User* user = network.findUser(username);
                User* postAuthor = network.findUser(friendUsername);
                if (user && postAuthor) {
                    cout << "Enter Post Index: ";
                    int postIndex;
                    cin >> postIndex;
                    if (postIndex >= 0 && postIndex < postAuthor->post.size()) {
                        cout << "Enter Comment: ";
                        cin.ignore();
                        getline(cin, comment);
                        user->commentOnPost(postAuthor->post[postIndex], comment);
                    } else {
                        cout << "Invalid Post Index!" << endl;
                    }
                } else {
                    cout << "One or both users not found!" << endl;
                }
            }
            break;

        case 11:
            network.displayAllPost();
            break;

        case 12:
            cout<<"Enter Authors Username: "<<endl;
            cin>>friendUsername;
            cout<<"Enter Post Index: "<<endl;
            int postIndex;
            cin>>postIndex;
            {
                User* author = network.findUser(friendUsername);
                if(author && postIndex >=0 && postIndex < author->post.size())
                {
                    author->post[postIndex].displayComments();
                }
                else{
                    cout<<"Either Post Index is Invalid or Author Does not exist!"<<endl;
                }
            
            }
            break;
        case 13:
            cout<<"Enter Your Username: ";
            cin>>username;
            {
                vector<string> recommendations = network.recommendFriends(username);
                if(!recommendations.empty())
                {
                    cout<<"Friend recommendations:\n";
                    for(const auto &rec: recommendations)
                    {
                        cout<<rec<<endl;
                    }
                }
                else
                {
                    cout<<"No recommendations available"<<endl;
                }
            }
            break;
        case 14:
    cout << "Enter search term: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore any leftover newline characters
    getline(cin, searchTerm);
    {
        vector<string> searchResults = network.searchUsers(searchTerm);
        if (!searchResults.empty()) {
            cout << "Search results:\n";
            for (const auto& result : searchResults) {
                cout << result << endl;
            }
        } else {
            cout << "No matching users found." << endl;
        }
    }
    break;


        case 15: 
            cout<<"Exiting..."<<endl;
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    }

    return 0;
}
