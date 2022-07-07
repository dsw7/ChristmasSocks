from concurrent import futures
from pytest import mark
import utils

def wrap_send(test_string: str, client: Client) -> dict:
    return {
        'outgoing_message': test_string,
        'returning_message': client.send(test_string)
    }

@mark.release_test
def test_echo_3_connections(socks_server_background, socks_three_clients) -> None:

    strings = utils.generate_random_string(num_strings=3, len_strings=5)
    workers, results = [], []

    with futures.ThreadPoolExecutor() as executor:

        workers.append(executor.submit(wrap_send, strings[0], self.client_a))
        workers.append(executor.submit(wrap_send, strings[1], self.client_b))
        workers.append(executor.submit(wrap_send, strings[2], self.client_c))

        if futures.wait(workers, return_when=futures.FIRST_COMPLETED):
            for worker in workers:
                results.append(worker.result())

    for result in results:
        assert result['outgoing_message'] == result['returning_message']
