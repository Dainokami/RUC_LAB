#include <iostream>
#include <algorithm>
#include <vector>
#include <limits> // for std::numeric_limits
#include "controller.hh"
#include "timestamp.hh"
using namespace std;

double rto = 100;
double sample_rtt = 0;
int increase_times = 0;
int decrease_times = 0;
// vector<uint64_t> rtt_time;
double alpha = 5.0;
double betas = 6.0;
double gamma = 2.0;
const double ALPHA_INCREMENT = 1.04999;
const double ALPHA_DECREMENT = 0.7025;
const double SLOW_START_INCREMENT = 1.2;
const double CONGESTION_AVOIDANCE_INCREMENT = 1.05;
double cwnd = 25; // 拥塞窗口
double ssthresh = 85; // 慢启动阈值
uint64_t last_base = 0;
uint64_t base_rtt = 0;
uint64_t min_rtt = numeric_limits<uint64_t>::max();

Controller::Controller(const bool debug)
    : debug_(debug) {}



/* Get current window size, in datagrams */
unsigned int Controller::window_size() {
    if (debug_) {
        cerr << "At time " << timestamp_ms() << " window size is " << cwnd << endl;
    }
    return static_cast<unsigned int>(cwnd);
}

/* A datagram was sent */
void Controller::datagram_was_sent(const uint64_t sequence_number,
                                   const uint64_t send_timestamp,
                                   const bool after_timeout) {
    if (debug_) {
        cerr << "At time " << send_timestamp
             << " sent datagram " << sequence_number << " (timeout = " << after_timeout << ")\n";
    }

  if(after_timeout){
    ssthresh = cwnd/2 + 1;
    cwnd = min(25.0,ssthresh/2);
  }

}

/* An ack was received */
void Controller::ack_received(const uint64_t sequence_number_acked,
                              const uint64_t send_timestamp_acked,
                              const uint64_t recv_timestamp_acked,
                              const uint64_t timestamp_ack_received) {
    if (debug_) {
        cerr << "At time " << timestamp_ack_received
             << " received ack for datagram " << sequence_number_acked
             << " (send @ time " << send_timestamp_acked
             << ", received @ time " << recv_timestamp_acked << " by receiver's clock)"
             << endl;
    }


    sample_rtt = timestamp_ack_received - send_timestamp_acked;

    if(min_rtt>sample_rtt)
        min_rtt = sample_rtt;

    if (min_rtt != base_rtt) {
        last_base = base_rtt;
        base_rtt = min_rtt;
    }



    double expected = cwnd / base_rtt;
    double actual = cwnd / sample_rtt;
    double diff = base_rtt * (expected - actual);
    double target = cwnd * base_rtt / last_base;



    if (cwnd < ssthresh) { // 慢启动
        if (static_cast<uint64_t>(diff) > gamma) {
            cwnd += min(static_cast<uint64_t>(cwnd), static_cast<uint64_t>(target) + 1);
        } else {
            cwnd += min(max(2 * cwnd, 2.0), SLOW_START_INCREMENT * (ssthresh - cwnd));
        }

    } else { // 拥塞避免
        if (diff < alpha) {
            increase_times++;
            decrease_times = 0;
            cwnd += CONGESTION_AVOIDANCE_INCREMENT;
        } else if (diff > betas) {
            decrease_times++;
            increase_times = 0;
            cwnd = max(1.0, cwnd - 0.1);
        }
    }



    if (increase_times >= 2) {
        alpha *= ALPHA_INCREMENT;
        betas *= ALPHA_INCREMENT;
        increase_times = 0;
        ssthresh *= 1.4;
    }
    
    if (decrease_times >= 2) {
        alpha *= ALPHA_DECREMENT;
        betas *= ALPHA_DECREMENT;
        decrease_times = 0;
        ssthresh *= 0.714;
    }
}


/* How long to wait (in milliseconds) if there are no acks
   before sending one more datagram */
unsigned int Controller::timeout_ms() {
    return static_cast<unsigned int>(rto);
}
