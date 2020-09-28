import sys
import os

sys.path.append(
    os.path.join(os.path.dirname(__file__), "./build", "lib.linux-x86_64-3.8"),
)

import whoami

print(whoami.whoami())
