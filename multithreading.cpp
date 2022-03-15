#include<iostream>
#include<fstream>
#include <vector>
#include <queue>
#include <set>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>


using namespace std;

string upload = "D:\\ÍÃÓ\\results\\";
string download = "D:\\ÍÃÓ\\tests\\";




class WebCrawler{
private:    
    int our_threads;
    vector<thread> threads;
    mutex contLock;
    atomic<int> used_threads;

    queue<string> we_need_to_download;
    set<string> downloaded;

    string first_site;    
public:
	WebCrawler(string site, int threads) : first_site(site), our_threads(threads){ 
        we_need_to_download.push(first_site);
        downloaded.insert(first_site);
	}
	
	void new_site_add (const string& site_name){
        lock_guard<mutex> lockGuard(contLock);
        if(downloaded.find(site_name) == downloaded.end()) 
            we_need_to_download.push(site_name);
    }

    pair<bool, string> get_new_site() {
        lock_guard<mutex> guard(contLock);

        if (we_need_to_download.empty() == 0) {
            string next_file = we_need_to_download.front();
            we_need_to_download.pop();
            return make_pair(true, next_file);
        }
        return make_pair(false, "");
    }
    
    
	
	void copy_site(const string& site_name){
        string to = site_name + upload ;
        string from = site_name + download ;
 		
    	ofstream outfile(to, ios::binary);
        ifstream infile(from, ios::binary);
    	
		outfile << infile.rdbuf();
		
		infile.close();
		outfile.close();
	}
	
    void site_add_done(const string& site_name) {
        lock_guard<mutex> lockGuard(contLock);
        downloaded.insert(site_name);
    }

	void analysis(const string& site_name){
		if(downloaded.find(site_name) != downloaded.end() && site_name != first_site) return;
		
		string buffer, address;
		vector<string> links;
 		
		ifstream analyzed_file(download + site_name);
    	copy_site(site_name);

        getline(analyzed_file, buffer);
		analyzed_file.close();
    	
    	int i = 0, start, finish, end = buffer.rfind("\">");
    	
    	do{
    		start = buffer.find("<a href=\"file://", i) + 16;
    		finish = buffer.find("\">", i);
    		address = buffer.substr(start, finish - start);
			links.push_back(address);
			i = finish + 1;
		}while(i < end);
		
		for(auto i : links) new_site_add(i);
		
		site_add_done(site_name);
	}
	
	void program(){
        while(!we_need_to_download.empty() || used_threads != 0){
            auto file = get_new_site();
            if(file.first){
                used_threads++;
                analysis(file.second);
                used_threads--;
            }
        }
    }

    int get_amount()
    {
        return downloaded.size();
    }
	
    double our_time = 0;
	void action(){
		used_threads = 0;
        clock_t start = clock();
        for(int i = 0; i < our_threads; ++i) 
            threads.push_back(thread(&WebCrawler::program, ref(*this)));
        for(int i = 0; i < our_threads; ++i) 
            threads[i].join();
		clock_t end = clock();
		our_time = end - start;
	}
	

	


};


int main(){
	ifstream fin("input.txt");
    ofstream fout("output.txt"); 
    string first_site;
    int our_threads;
    for (size_t i = 0; i < 8; i++)
    {
        fin >> first_site >> our_threads;
        WebCrawler crawler(first_site, our_threads);
        crawler.action();
        fout << crawler.get_amount() << "   " << crawler.our_time << endl;
    }
    fin.close();
    fout.close();
    return 0;
}
