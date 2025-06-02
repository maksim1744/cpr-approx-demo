import shutil
import os

os.makedirs("archive", exist_ok=True)
for i in range(1, 13):
    try:
        shutil.copy(f"tests/{i:0>3}.ans", f"archive/{i:0>2}.out")
    except:
        pass

shutil.make_archive("archive", 'zip', "archive")
shutil.rmtree("archive")
