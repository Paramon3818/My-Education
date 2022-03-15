#include <iostream>
#include <string>
#include <map>
#include <stack>
#include <queue>

using namespace std;

map< char, string > makeMap()
{
	char c;
	string s;
	map <char, string> myMap;
	for (int i = 0; i < 4; i++) {
		cin >> c;
		getline(cin, s, '\n');
		myMap[c] = s;
	}
	myMap[')'] = 4;
	myMap['('] = 5;

	return myMap;
}

queue<char> makeCorrectQueue(int length, string expression, map<char, string > myMap)
{
	queue<char> queue_1;
	stack <char> helper;
	char top;
	string value_1, value_2;
	for (int i = 0; i < length; i++) {
		if (expression[i] >= '0' && expression[i] <= '9') queue_1.push(expression[i]);
		else if (expression[i] == '(') helper.push(expression[i]);
		else if (expression[i] == ')') {
			while (helper.top() != '(') {
				queue_1.push(helper.top());
				helper.pop();
			}
			helper.pop();
		}
		else
		if (expression[i] == '+' || expression[i] == '-' || expression[i] == '/' || expression[i] == '*') {
			if (!helper.empty() && helper.top() != '(' && helper.top() != ')') {
				top = helper.top();
				value_1 = myMap[top];
				value_2 = myMap[expression[i]];
				while (((value_1[1] ) >= (value_2[1] ) && (!helper.empty()) && helper.top() != '(' && helper.top() != ')')) {
					if (((value_1[1] ) == (value_2[1])) && (value_2[3] == 'L') || ((value_1[1] ) > (value_2[1] ))) {
						queue_1.push(top);
						helper.pop();
						if (!helper.empty()) {
							top = helper.top();
							value_1 = myMap[top];
						}
					}
					else break;
				}
			}
			helper.push(expression[i]);

		}
		
	}
	while (!helper.empty()) {
		queue_1.push(helper.top());
		helper.pop();
	}
	return queue_1;
}

void makeandprintAnswer(queue<char> myQueue)
{
	try {
		int top;
		stack <int> answer;
		while (!myQueue.empty()) {
			if (myQueue.front() >= '0' && myQueue.front() <= '9') {
				answer.push(myQueue.front() - '0');
				myQueue.pop();
			}
			else if (myQueue.front() == '+' || myQueue.front() == '-' || myQueue.front() == '/' || myQueue.front() == '*') {
				top = answer.top();
				
				answer.pop();
				if (myQueue.front() == '+') top = top + answer.top();
				else if (myQueue.front() == '-') top = answer.top() - top;
				else if (myQueue.front() == '*') top = top * answer.top();
				else if (myQueue.front() == '/') {
					if (top == 0)
						throw - 1;
					top = answer.top() / top;
				}
				answer.pop();
				answer.push(top);
				myQueue.pop();
			}

		}
		cout << answer.top();
	}
	catch (int a)
	{
		cout << "ERROR";
	}
}

int main() {
	map<char, string>myMap = makeMap();
	string expression;
	getline(cin, expression, '\n');
	int length_1 = expression.length();
	queue<char> myQueue = makeCorrectQueue(length_1, expression, myMap);
	makeandprintAnswer(myQueue);
}



/*+ 0 R
- 0 L
* 1 L
/ 1 L*/