import os
import shutil
from pathlib import Path

Import("env")

def copy_files_to_build(source, target, env):
    data_dir = Path(env['PROJECT_DIR']) / 'data'
    build_dir = Path(env['BUILD_DIR'])
    
    if data_dir.exists():
        for file in data_dir.glob('*'):
            if file.is_file():
                dest = build_dir / 'spiffs' / file.name
                dest.parent.mkdir(parents=True, exist_ok=True)
                shutil.copy2(file, dest)
                print(f"Copied {file.name} to SPIFFS image")

env.AddPostAction("$BUILD_DIR/spiffs.bin", copy_files_to_build)
